#include "sqpcheader.h"
#ifndef NO_COMPILER
#include "opcodes.h"
#include "sqstring.h"
#include "sqfuncproto.h"
#include "parser.h"
#include "compiler.h"
#include "compilationcontext.h"
#include <stdarg.h>

namespace SQCompilation {

struct NestingChecker {
    SQParser *_p;
    const uint32_t _max_depth;
    uint32_t _depth;
    NestingChecker(SQParser *p) : _p(p), _depth(0), _max_depth(500) {
        inc();
    }

    ~NestingChecker() {
        _p->_depth -= _depth;
    }

    void inc() {
        if (_p->_depth > _max_depth) {
            _p->reportDiagnostic(DiagnosticsId::DI_TOO_BIG_AST);
        }
        _p->_depth += 1;
        _depth += 1;
    }
};

SQParser::SQParser(SQVM *v, const char *sourceText, size_t sourceTextSize, const SQChar* sourcename, Arena *astArena, SQCompilationContext &ctx, Comments *comments)
    : _lex(_ss(v), ctx, comments)
    , _ctx(ctx)
    , _astArena(astArena)
    , _docObjectStack(astArena)
{
    _vm=v;
    _lex.Init(sourceText, sourceTextSize);
    _sourcename = sourcename;
    _expression_context = SQE_REGULAR;
    _lang_features = _ss(v)->defaultLangFeatures;
    _depth = 0;
    _token = 0;
    _docObjectStack.push_back(&_moduleDocObject);
}


void SQParser::reportDiagnostic(int32_t id, ...) {
    va_list vargs;
    va_start(vargs, id);

    _ctx.vreportDiagnostic((enum DiagnosticsId)id, line(), column(), width(), vargs);

    va_end(vargs);
}


bool SQParser::ProcessPosDirective()
{
    const SQChar *sval = _lex._svalue;
    if (strncmp(sval, _SC("pos:"), 4) != 0)
        return false;

    sval += 4;
    if (!isdigit(*sval))
        reportDiagnostic(DiagnosticsId::DI_EXPECTED_LINENUM);
    SQChar * next = NULL;
    _lex._currentline = scstrtol(sval, &next, 10);
    if (!next || *next != ':') {
        reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, ":");
        return false;
    }
    next++;
    if (!isdigit(*next))
        reportDiagnostic(DiagnosticsId::DI_EXPECTED_COLNUM);
    _lex._currentcolumn = scstrtol(next, NULL, 10);
    return true;
}

struct SQPragmaDescriptor {
  const SQChar *id;
  SQInteger setFlags, clearFlags;
};

static const SQPragmaDescriptor pragmaDescriptors[] = {
  { "strict", LF_STRICT, 0 },
  { "relaxed", 0, LF_STRICT },
  { "forbid-root-table", LF_FORBID_ROOT_TABLE, 0 },
  { "allow-root-table", 0, LF_FORBID_ROOT_TABLE },
  { "disable-optimizer", LF_DISABLE_OPTIMIZER, 0 },
  { "enable-optimizer", 0, LF_DISABLE_OPTIMIZER },
  { "forbid-delete-operator", LF_FORBID_DELETE_OP, 0 },
  { "allow-delete-operator", 0, LF_FORBID_DELETE_OP },
  { "forbid-clone-operator", LF_FORBID_CLONE_OP, 0 },
  { "allow-clone-operator", 0, LF_FORBID_CLONE_OP },
  { "forbid-switch-statement", LF_FORBID_SWITCH_STMT, 0 },
  { "allow-switch-statement", 0, LF_FORBID_SWITCH_STMT },
  { "forbid-implicit-default-delegates", LF_FORBID_IMPLICIT_DEF_DELEGATE, 0 },
  { "allow-implicit-default-delegates", 0, LF_FORBID_IMPLICIT_DEF_DELEGATE },
  { "forbid-auto-freeze", 0, LF_ALLOW_AUTO_FREEZE },
  { "allow-auto-freeze", LF_ALLOW_AUTO_FREEZE, 0 },
};

Statement* SQParser::parseDirectiveStatement()
{
    const SQChar *sval = _lex._svalue;

    bool applyToDefault = false;
    if (strncmp(sval, _SC("default:"), 8) == 0) {
        applyToDefault = true;
        sval += 8;
    }

    const SQPragmaDescriptor *pragmaDesc = nullptr;

    for (const auto &desc : pragmaDescriptors) {
      if (strcmp(sval, desc.id) == 0) {
        pragmaDesc = &desc;
        break;
      }
    }

    if (pragmaDesc == nullptr) {
      reportDiagnostic(DiagnosticsId::DI_UNSUPPORTED_DIRECTIVE, sval);
      return nullptr;
    }

    SQInteger setFlags = pragmaDesc->setFlags, clearFlags = pragmaDesc->clearFlags;

    DirectiveStmt *d = newNode<DirectiveStmt>();
    d->setLineStartPos(_lex._tokenline);
    d->setColumnStartPos(_lex._tokencolumn);
    d->setLineEndPos(_lex._currentline);
    d->setColumnEndPos(_lex._currentcolumn);

    d->applyToDefault = applyToDefault;
    d->setFlags = setFlags;
    d->clearFlags = clearFlags;

    _lang_features = (_lang_features | setFlags) & ~clearFlags;
    if (applyToDefault)
        _ss(_vm)->defaultLangFeatures = (_ss(_vm)->defaultLangFeatures | setFlags) & ~clearFlags;

    Lex();
    return d;
}

void SQParser::checkBraceIndentationStyle()
{
  if (_token == _SC('{') && (_lex._prevflags & TF_PREP_EOL))
    reportDiagnostic(DiagnosticsId::DI_EGYPT_BRACES);
}

void SQParser::Lex()
{
    _token = _lex.Lex();

    while (_token == TK_DIRECTIVE)
    {
        bool endOfLine = (_lex._prevtoken == _SC('\n'));
        if (ProcessPosDirective()) {
            _token = _lex.Lex();
            if (endOfLine)
                _lex._prevtoken = _SC('\n');
        } else
            break;
    }

    const bool forceIdentifier =
           (_token == TK_CLONE && (_lang_features & LF_FORBID_CLONE_OP))
        || ((_token == TK_SWITCH || _token == TK_CASE || _token == TK_DEFAULT) && (_lang_features & LF_FORBID_SWITCH_STMT));

    if (forceIdentifier) {
      _token = TK_IDENTIFIER;
      _lex.SetStringValue();
    }
}


Expr* SQParser::Expect(SQInteger tok)
{
    if(_token != tok) {
        if(_token == TK_CONSTRUCTOR && tok == TK_IDENTIFIER) {
            //do nothing
        }
        else {
            if(tok > 255) {
                const SQChar *etypename;
                switch(tok)
                {
                case TK_IDENTIFIER:
                    etypename = _SC("IDENTIFIER");
                    break;
                case TK_STRING_LITERAL:
                    etypename = _SC("STRING_LITERAL");
                    break;
                case TK_INTEGER:
                    etypename = _SC("INTEGER");
                    break;
                case TK_FLOAT:
                    etypename = _SC("FLOAT");
                    break;
                default:
                    etypename = _lex.Tok2Str(tok);
                }
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, etypename);
            }
            else {
                char s[2] = {(char)tok, 0};
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, s);
            }
        }
    }
    Expr *ret = NULL;
    switch(tok)
    {
    case TK_IDENTIFIER:
        ret = newId(_lex._svalue);
        break;
    case TK_STRING_LITERAL:
        ret = newStringLiteral(_lex._svalue);
        break;
    case TK_INTEGER:
        ret = setCoordinates(newNode<LiteralExpr>(_lex._nvalue), line(), column());
        break;
    case TK_FLOAT:
        ret = setCoordinates(newNode<LiteralExpr>(_lex._fvalue), line(), column());
        break;
    }
    Lex();
    return ret;
}


void SQParser::OptionalSemicolon()
{
    if(_token == _SC(';')) { Lex(); return; }
    if(!IsEndOfStatement()) {
        reportDiagnostic(DiagnosticsId::DI_END_OF_STMT_EXPECTED);
    }
}


RootBlock* SQParser::parse()
{
    try {
        RootBlock *rootBlock = newNode<RootBlock>(arena());
        Lex();
        rootBlock->setLineStartPos(line());
        rootBlock->setColumnStartPos(column());
        while(_token > 0){
            rootBlock->addStatement(parseStatement());
            if(_lex._prevtoken != _SC('}') && _lex._prevtoken != _SC(';')) OptionalSemicolon();
        }
        rootBlock->setLineEndPos(_lex._currentline);
        rootBlock->setColumnEndPos(_lex._currentcolumn);
        return rootBlock;
    }
    catch (SQCompilation::CompilerError &) {
        return NULL;
    }
}


Block* SQParser::parseStatements()
{
    NestingChecker nc(this);
    Block *result = newNode<Block>(arena());
    while(_token != _SC('}') && _token != TK_DEFAULT && _token != TK_CASE) {
        Statement *stmt = parseStatement();
        result->addStatement(stmt);
        if(_lex._prevtoken != _SC('}') && _lex._prevtoken != _SC(';')) OptionalSemicolon();
    }
    return result;
}


void SQParser::onDocString(const SQChar *doc_string)
{
    if (_docObjectStack.back()->getDocString() != nullptr)
        reportDiagnostic(DiagnosticsId::DI_MULTIPLE_DOCSTRINGS);
    else {
        SQString *docStringCopy = SQString::Create(_ss(_vm), doc_string);
        if (docStringCopy)
            _docObjectStack.back()->setDocString(docStringCopy->_val);
    }
}


Statement* SQParser::parseStatement(bool closeframe)
{
    NestingChecker nc(this);
    Statement *result = NULL;
    SQInteger l = line(), c = column();

    bool allowsImport = false;

    switch(_token) {
    case _SC(';'):
        allowsImport = true;
        result = newNode<EmptyStatement>();
        Lex();
        break;
    case TK_DOCSTRING: {
        onDocString(_lex._svalue);
        result = newNode<EmptyStatement>();
        Lex();
        break;
    }
    case TK_DIRECTIVE:
        allowsImport = true;
        result = parseDirectiveStatement();
        break;
    case TK_IF:     result = parseIfStatement();          break;
    case TK_WHILE:  result = parseWhileStatement();       break;
    case TK_DO:     result = parseDoWhileStatement();     break;
    case TK_FOR:    result = parseForStatement();         break;
    case TK_FOREACH:result = parseForEachStatement();     break;
    case TK_SWITCH: result = parseSwitchStatement();      break;
    case TK_LOCAL:
    case TK_LET:
        result = parseLocalDeclStatement();
        break;
    case TK_RETURN:
    case TK_YIELD: {
        SQOpcode op;
        if(_token == TK_RETURN) {
            op = _OP_RETURN;
        }
        else {
            op = _OP_YIELD;
        }
        SQInteger le = _lex._currentline;
        SQInteger ce = _lex._currentcolumn;
        Lex();

        Expr *arg = NULL;
        if(!IsEndOfStatement()) {
            arg = Expression(SQE_RVALUE);
        }

        result = op == _OP_RETURN ? static_cast<TerminateStatement *>(newNode<ReturnStatement>(arg)) : newNode<YieldStatement>(arg);
        if (arg) {
            copyCoordinates(arg, result);
        }
        else {
            result->setLineEndPos(le);
            result->setColumnEndPos(ce);
        }

        result->setLineStartPos(l);
        result->setColumnStartPos(c);

        return result;
    }
    case TK_BREAK:
        result = setCoordinates(newNode<BreakStatement>(nullptr), l, c);
        Lex();
        return result;
    case TK_CONTINUE:
        result = setCoordinates(newNode<ContinueStatement>(nullptr), l, c);
        Lex();
        return result;
    case TK_FUNCTION:
        result = parseLocalFunctionDeclStmt(false);
        break;
    case TK_CLASS:
        result = parseLocalClassDeclStmt(false);
        break;
    case TK_ENUM:
        result = parseEnumStatement(false);
        break;
    case _SC('{'):
    {
        SQUnsignedInteger savedLangFeatures = _lang_features;
        Lex();
        result = parseStatements();
        Expect(_SC('}'));
        _lang_features = savedLangFeatures;
        break;
    }
    case TK_TRY:
        result = parseTryCatchStatement();
        break;
    case TK_THROW: {
        Lex();
        Expr *e = Expression(SQE_RVALUE);
        result = copyCoordinates(e, newNode<ThrowStatement>(e));
        result->setLineStartPos(l); result->setColumnStartPos(c);
        return result;
    }
    case TK_CONST:
        result = parseConstStatement(false);
        break;
    case TK_GLOBAL:
        Lex();
        if (_token == TK_CONST)
            result = parseConstStatement(true);
        else if (_token == TK_ENUM)
            result = parseEnumStatement(true);
        else
            reportDiagnostic(DiagnosticsId::DI_GLOBAL_CONSTS_ONLY);
        break;
    default: {
        // Check for contextual import keywords to switch parsing mode
        if (_are_imports_still_allowed && _token == TK_IDENTIFIER) {
            if (strcmp(_lex._svalue, "import") == 0 || strcmp(_lex._svalue, "from") == 0) {
                allowsImport = true;
                result = parseImportStatement();
                break;
            }
        }
        Expr *e = Expression(SQE_REGULAR);
        return copyCoordinates(e, newNode<ExprStatement>(e));
      }
    }

    assert(result);

    setCoordinates(result, l, c);
    if (!allowsImport)
        _are_imports_still_allowed = false;

    return result;
}


Expr* SQParser::parseCommaExpr(SQExpressionContext expression_context)
{
    NestingChecker nc(this);
    Expr *expr = Expression(expression_context);

    if (_token == ',') {
        CommaExpr *cm = newNode<CommaExpr>(arena());
        cm->addExpression(expr);
        expr = cm;
        while (_token == ',') {
            Lex();
            cm->addExpression(Expression(expression_context));
        }
    }

    return expr;
}


Expr* SQParser::Expression(SQExpressionContext expression_context)
{
    NestingChecker nc(this);
    SQExpressionContext saved_expression_context = _expression_context;
    _expression_context = expression_context;

    Expr *expr = LogicalNullCoalesceExp();

    switch(_token)  {
    case _SC('='):
    case TK_NEWSLOT:
    case TK_MINUSEQ:
    case TK_PLUSEQ:
    case TK_MULEQ:
    case TK_DIVEQ:
    case TK_MODEQ: {
        SQInteger op = _token;
        Lex();
        Expr *e2 = Expression(SQE_RVALUE);

        switch (op) {
        case TK_NEWSLOT:
            expr = newNode<BinExpr>(TO_NEWSLOT, expr, e2);
            break;
        case _SC('='): //ASSIGN
            switch (expression_context)
            {
            case SQE_IF:
                reportDiagnostic(DiagnosticsId::DI_ASSIGN_INSIDE_FORBIDDEN, "if");
                break;
            case SQE_LOOP_CONDITION:
                reportDiagnostic(DiagnosticsId::DI_ASSIGN_INSIDE_FORBIDDEN, "loop condition");
                break;
            case SQE_SWITCH:
                reportDiagnostic(DiagnosticsId::DI_ASSIGN_INSIDE_FORBIDDEN, "switch");
                break;
            case SQE_FUNCTION_ARG:
                reportDiagnostic(DiagnosticsId::DI_ASSIGN_INSIDE_FORBIDDEN, "function argument");
                break;
            case SQE_RVALUE:
                reportDiagnostic(DiagnosticsId::DI_ASSIGN_INSIDE_FORBIDDEN, "expression");
                break;
            case SQE_ARRAY_ELEM:
                reportDiagnostic(DiagnosticsId::DI_ASSIGN_INSIDE_FORBIDDEN, "array element");
                break;
            case SQE_REGULAR:
                break;
            }
            expr = newNode<BinExpr>(TO_ASSIGN, expr, e2);
            break;
        case TK_MINUSEQ: expr = newNode<BinExpr>(TO_MINUSEQ, expr, e2); break;
        case TK_PLUSEQ: expr = newNode<BinExpr>(TO_PLUSEQ, expr, e2); break;
        case TK_MULEQ: expr = newNode<BinExpr>(TO_MULEQ, expr, e2); break;
        case TK_DIVEQ: expr = newNode<BinExpr>(TO_DIVEQ, expr, e2); break;
        case TK_MODEQ: expr = newNode<BinExpr>(TO_MODEQ, expr, e2); break;
        }
    }
    break;
    case _SC('?'): {
        Consume('?');

        Expr *ifTrue = Expression(SQE_RVALUE);

        Expect(_SC(':'));

        Expr *ifFalse = Expression(SQE_RVALUE);

        expr = newNode<TerExpr>(expr, ifTrue, ifFalse);
    }
    break;
    }

    _expression_context = saved_expression_context;
    return expr;
}


template<typename T> Expr* SQParser::BIN_EXP(T f, enum TreeOp top, Expr *lhs)
{

    SQInteger prevTok = _lex._prevtoken, tok = _token;
    unsigned prevFlags = _lex._prevflags;

    Lex();

    checkSuspiciousUnaryOp(prevTok, tok, prevFlags);

    SQExpressionContext old = _expression_context;
    _expression_context = SQE_RVALUE;

    Expr *rhs = (this->*f)();

    _expression_context = old;

    return newNode<BinExpr>(top, lhs, rhs);
}


Expr* SQParser::LogicalNullCoalesceExp()
{
    NestingChecker nc(this);
    Expr *lhs = LogicalOrExp();
    for (;;) {
        nc.inc();
        if (_token == TK_NULLCOALESCE) {
            Lex();

            Expr *rhs = LogicalNullCoalesceExp();
            lhs = newNode<BinExpr>(TO_NULLC, lhs, rhs);
        }
        else return lhs;
    }
}


Expr* SQParser::LogicalOrExp()
{
    NestingChecker nc(this);
    Expr *lhs = LogicalAndExp();
    for (;;) {
        nc.inc();
        if (_token == TK_OR) {
            Lex();

            Expr *rhs = LogicalOrExp();
            lhs = newNode<BinExpr>(TO_OROR, lhs, rhs);
        }
        else return lhs;
    }
}

Expr* SQParser::LogicalAndExp()
{
    NestingChecker nc(this);
    Expr *lhs = BitwiseOrExp();
    for (;;) {
        nc.inc();
        switch (_token) {
        case TK_AND: {
            Lex();

            Expr *rhs = LogicalAndExp();
            lhs = newNode<BinExpr>(TO_ANDAND, lhs, rhs);
        }
        default:
            return lhs;
        }
    }
}

Expr* SQParser::BitwiseOrExp()
{
    NestingChecker nc(this);
    Expr *lhs = BitwiseXorExp();
    for (;;) {
        nc.inc();
        if (_token == _SC('|')) {
            return BIN_EXP(&SQParser::BitwiseOrExp, TO_OR, lhs);
        }
        else return lhs;
    }
}

Expr* SQParser::BitwiseXorExp()
{
    NestingChecker nc(this);
    Expr * lhs = BitwiseAndExp();
    for (;;) {
        nc.inc();
        if (_token == _SC('^')) {
            lhs = BIN_EXP(&SQParser::BitwiseAndExp, TO_XOR, lhs);
        }
        else return lhs;
    }
}

Expr* SQParser::BitwiseAndExp()
{
    NestingChecker nc(this);
    Expr *lhs = EqExp();
    for (;;) {
        nc.inc();
        if (_token == _SC('&')) {
            lhs = BIN_EXP(&SQParser::EqExp, TO_AND, lhs);
        }
        else return lhs;
    }
}

Expr* SQParser::EqExp()
{
    NestingChecker nc(this);
    Expr *lhs = CompExp();
    for (;;) {
        nc.inc();
        switch (_token) {
        case TK_EQ: lhs = BIN_EXP(&SQParser::CompExp, TO_EQ, lhs); break;
        case TK_NE: lhs = BIN_EXP(&SQParser::CompExp, TO_NE, lhs); break;
        case TK_3WAYSCMP: lhs = BIN_EXP(&SQParser::CompExp, TO_3CMP, lhs); break;
        default: return lhs;
        }
    }
}

Expr* SQParser::CompExp()
{
    NestingChecker nc(this);
    Expr *lhs = ShiftExp();
    for (;;) {
        nc.inc();
        switch (_token) {
        case _SC('>'): lhs = BIN_EXP(&SQParser::ShiftExp, TO_GT, lhs); break;
        case _SC('<'): lhs = BIN_EXP(&SQParser::ShiftExp, TO_LT, lhs); break;
        case TK_GE: lhs = BIN_EXP(&SQParser::ShiftExp, TO_GE, lhs); break;
        case TK_LE: lhs = BIN_EXP(&SQParser::ShiftExp, TO_LE, lhs); break;
        case TK_IN: lhs = BIN_EXP(&SQParser::ShiftExp, TO_IN, lhs); break;
        case TK_INSTANCEOF: lhs = BIN_EXP(&SQParser::ShiftExp, TO_INSTANCEOF, lhs); break;
        case TK_NOT: {
            Lex();
            SQInteger l = line(), c = column();
            if (_token == TK_IN) {
                lhs = BIN_EXP(&SQParser::ShiftExp, TO_IN, lhs);
                lhs = setCoordinates(newNode<UnExpr>(TO_NOT, lhs), l, c);
            }
            else
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "in");
        }
        default: return lhs;
        }
    }
}

Expr* SQParser::ShiftExp()
{
    NestingChecker nc(this);
    Expr *lhs = PlusExp();
    for (;;) {
        nc.inc();
        switch (_token) {
        case TK_USHIFTR: lhs = BIN_EXP(&SQParser::PlusExp, TO_USHR, lhs); break;
        case TK_SHIFTL: lhs = BIN_EXP(&SQParser::PlusExp, TO_SHL, lhs); break;
        case TK_SHIFTR: lhs = BIN_EXP(&SQParser::PlusExp, TO_SHR, lhs); break;
        default: return lhs;
        }
    }
}

void SQParser::checkSuspiciousUnaryOp(SQInteger pprevTok, SQInteger tok, unsigned pprevFlags) {
  if (tok == _SC('+') || tok == _SC('-')) {
    if (_expression_context == SQE_ARRAY_ELEM || _expression_context == SQE_FUNCTION_ARG) {
      if ((pprevFlags & (TF_PREP_EOL | TF_PREP_SPACE)) && (pprevTok != _SC(',')) && ((_lex._prevflags & TF_PREP_SPACE) == 0)) {
        reportDiagnostic(DiagnosticsId::DI_NOT_UNARY_OP, tok == _SC('+') ? "+" : "-");
      }
    }
  }
}

Expr* SQParser::PlusExp()
{
    NestingChecker nc(this);
    Expr *lhs = MultExp();
    for (;;) {
        nc.inc();
        switch (_token) {
        case _SC('+'): lhs = BIN_EXP(&SQParser::MultExp, TO_ADD, lhs); break;
        case _SC('-'): lhs = BIN_EXP(&SQParser::MultExp, TO_SUB, lhs); break;

        default: return lhs;
        }
    }
}

Expr* SQParser::MultExp()
{
    NestingChecker nc(this);
    Expr *lhs = PrefixedExpr();
    for (;;) {
        nc.inc();
        switch (_token) {
        case _SC('*'): lhs = BIN_EXP(&SQParser::PrefixedExpr, TO_MUL, lhs); break;
        case _SC('/'): lhs = BIN_EXP(&SQParser::PrefixedExpr, TO_DIV, lhs); break;
        case _SC('%'): lhs = BIN_EXP(&SQParser::PrefixedExpr, TO_MOD, lhs); break;

        default: return lhs;
        }
    }
}

void SQParser::checkSuspiciousBracket() {
  assert(_token == _SC('(') || _token == _SC('['));
  if (_expression_context == SQE_ARRAY_ELEM || _expression_context == SQE_FUNCTION_ARG) {
    if (_lex._prevtoken != _SC(',')) {
      if (_lex._prevflags & (TF_PREP_EOL | TF_PREP_SPACE)) {
        char op[] = { (char)_token, '\0' };
        reportDiagnostic(DiagnosticsId::DI_SUSPICIOUS_BRACKET, op, _token == _SC('(') ? "function call" : "access to member");
      }
    }
  }
}

static const char *opname(SQInteger op) {
  switch (op)
  {
    case _SC('.'): return ".";
    case TK_NULLGETSTR: return "?.";
    case TK_TYPE_METHOD_GETSTR: return ".$";
    case TK_NULLABLE_TYPE_METHOD_GETSTR: return "?.$";
    default: return "<unknown>";
  }
}

Expr* SQParser::PrefixedExpr()
{
    NestingChecker nc(this);
    //if 'pos' != -1 the previous variable is a local variable
    SQInteger pos;
    Expr *e = Factor(pos);
    bool nextIsNullable = false;
    for(;;) {
        nc.inc();
        switch(_token) {
        case _SC('.'):
        case TK_NULLGETSTR:
        case TK_TYPE_METHOD_GETSTR:
        case TK_NULLABLE_TYPE_METHOD_GETSTR: {
            if (_token == TK_NULLGETSTR || _token == TK_NULLABLE_TYPE_METHOD_GETSTR || nextIsNullable) {
                nextIsNullable = true;
            }

            bool isTypeMethod = _token == TK_TYPE_METHOD_GETSTR || _token == TK_NULLABLE_TYPE_METHOD_GETSTR;

            SQInteger tok = _token;

            Lex();

            if ((_lex._prevflags & (TF_PREP_SPACE | TF_PREP_EOL)) != 0) {
              reportDiagnostic(DiagnosticsId::DI_SPACE_SEP_FIELD_NAME, opname(tok));
            }

            SQInteger l = _lex._currentline, c = _lex._currentcolumn;
            Expr *receiver = e;
            Id *id = (Id *)Expect(TK_IDENTIFIER);
            assert(id);
            e = newNode<GetFieldExpr>(receiver, id->name(), nextIsNullable, isTypeMethod); //-V522
            e->setLineStartPos(receiver->lineStart()); e->setColumnStartPos(receiver->columnStart());
            e->setLineEndPos(l); e->setColumnEndPos(c);
            break;
        }
        case _SC('['):
        case TK_NULLGETOBJ: {
            if (_token == TK_NULLGETOBJ || nextIsNullable)
            {
                nextIsNullable = true;
            }
            if(_lex._prevtoken == _SC('\n'))
                reportDiagnostic(DiagnosticsId::DI_BROKEN_SLOT_DECLARATION);
            if (_token == _SC('['))
              checkSuspiciousBracket();

            Lex();
            Expr *receiver = e;
            Expr *key = Expression(SQE_RVALUE);
            e = setCoordinates(newNode<GetSlotExpr>(receiver, key, nextIsNullable), receiver->lineStart(), receiver->columnStart());
            Expect(_SC(']'));
            break;
        }
        case TK_MINUSMINUS:
        case TK_PLUSPLUS:
            {
                nextIsNullable = false;
                if(IsEndOfStatement()) return e;
                SQInteger diff = (_token==TK_MINUSMINUS) ? -1 : 1;
                e = setCoordinates(newNode<IncExpr>(e, diff, IF_POSTFIX), e->lineStart(), e->columnStart());
                Lex();
            }
            return e;
        case _SC('('):
        case TK_NULLCALL: {
            if (_lex._prevflags & TF_PREP_EOL && _token == _SC('(')) {
                reportDiagnostic(DiagnosticsId::DI_PAREN_IS_FUNC_CALL);
            }
            SQInteger nullcall = (_token==TK_NULLCALL || nextIsNullable);
            nextIsNullable = !!nullcall;
            SQInteger l = e->lineStart(), c = e->columnStart();
            CallExpr *call = newNode<CallExpr>(arena(), e, nullcall);

            if (_token == _SC('('))
              checkSuspiciousBracket();

            Lex();
            while (_token != _SC(')')) {
                call->addArgument(Expression(SQE_FUNCTION_ARG));
                if (_token == _SC(',')) {
                    Lex();
                }
            }

            setCoordinates(call, l, c);

            Lex();
            e = call;
            break;
        }
        default: return e;
        }
    }
}

static void appendStringData(sqvector<SQChar> &dst, const SQChar *b) {
  while (*b) {
    dst.push_back(*b++);
  }
}

Expr *SQParser::parseStringTemplate() {

    // '$' TK_TEMPLATE_PREFIX? (arg TK_TEMPLATE_INFIX)* arg? TK_TEMPLATE_SUFFX

    _lex._state = LS_TEMPLATE;
    _lex._expectedToken = TK_TEMPLATE_PREFIX;

    SQInteger l = line(), c = column();

    Lex();
    int idx = 0;

    sqvector<SQChar> formatString(_ctx.allocContext());
    sqvector<Expr *> args(_ctx.allocContext());
    char buffer[64] = {0};

    SQInteger fmtL = line(), fmtC = column();
    SQInteger tok = -1;

    while ((tok = _token) != SQUIRREL_EOB) {

      if (tok != TK_TEMPLATE_PREFIX && tok != TK_TEMPLATE_INFIX && tok != TK_TEMPLATE_SUFFIX) {
          reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, _SC("STRING_LITERAL"));
          return nullptr;
      }

      appendStringData(formatString, _lex._svalue);
      if (tok != TK_TEMPLATE_SUFFIX) {
        snprintf(buffer, sizeof buffer, "%d", idx++);
        appendStringData(formatString, buffer);
        _lex._expectedToken = -1;
        _lex._state = LS_REGULAR;
        Lex();
        Expr *arg = Expression(SQE_FUNCTION_ARG);
        args.push_back(arg);

        if (_token != _SC('}')) {
            reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, _SC("}"));
            return nullptr;
        }

        formatString.push_back(_SC('}'));

        _lex._state = LS_TEMPLATE;
        _lex._expectedToken = TK_TEMPLATE_INFIX;
      }
      else {
        break;
      }
      Lex();
    }

    formatString.push_back('\0');

    Expr *result = nullptr;
    LiteralExpr *fmt = setCoordinates(newStringLiteral(&formatString[0]), fmtL, fmtC);

    if (args.empty()) {
      result = fmt;
    }
    else {
      Expr *callee = setCoordinates(newNode<GetFieldExpr>(fmt, "subst", false, /* force type method */ true), l, c);
      CallExpr *call = setCoordinates(newNode<CallExpr>(arena(), callee, false), l, c);

      for (Expr *arg : args)
        call->addArgument(arg);

      result = call;
    }

    _lex._expectedToken = -1;
    _lex._state = LS_REGULAR;
    Lex();

    return result;
}

Expr* SQParser::Factor(SQInteger &pos)
{
    NestingChecker nc(this);
    Expr *r = NULL;

    SQInteger l = line(), c = column();

    switch(_token)
    {
    case TK_STRING_LITERAL:
        r = newStringLiteral(_lex._svalue);
        r->setLineStartPos(l); r->setColumnStartPos(c);
        Lex();
        break;
    case TK_TEMPLATE_OP:
        r = parseStringTemplate();
        break;
    case TK_BASE:
        r = setCoordinates(newNode<BaseExpr>(), l, c);
        Lex();
        break;
    case TK_IDENTIFIER:
        r = newId(_lex._svalue);
        r->setLineStartPos(l); r->setColumnStartPos(c);
        Lex();
        break;
    case TK_CONSTRUCTOR:
        r = setCoordinates(newNode<Id>(_SC("constructor")), l, c);
        Lex();
        break;
    case TK_THIS: r = setCoordinates(newNode<Id>(_SC("this")), l, c); Lex(); break;
    case TK_DOUBLE_COLON:  // "::"
        if (_lang_features & LF_FORBID_ROOT_TABLE)
            reportDiagnostic(DiagnosticsId::DI_ROOT_TABLE_FORBIDDEN);
        _token = _SC('.'); /* hack: drop into PrefixExpr, case '.'*/
        r = setCoordinates(newNode<RootTableAccessExpr>(), l, c);
        break;
    case TK_NULL:
        r = setCoordinates(newNode<LiteralExpr>(), l, c);
        Lex();
        break;
    case TK_INTEGER:
        r = setCoordinates(newNode<LiteralExpr>(_lex._nvalue), l, c);
        Lex();
        break;
    case TK_FLOAT:
        r = setCoordinates(newNode<LiteralExpr>(_lex._fvalue), l, c);
        Lex();
        break;
    case TK_TRUE: case TK_FALSE:
        r = setCoordinates(newNode<LiteralExpr>((bool)(_token == TK_TRUE)), l, c);
        Lex();
        break;
    case _SC('['): {
            Lex();
            ArrayExpr *arr = newNode<ArrayExpr>(arena());
            bool commaSeparated = false;
            bool spaceSeparated = false;
            bool reported = false;
            while(_token != _SC(']')) {
                Expr *v = Expression(SQE_ARRAY_ELEM);
                arr->addValue(v);
                if (_token == _SC(',')) {
                    commaSeparated = true;
                }
                else if (_token != _SC(']')) {
                    spaceSeparated = true;
                }

                if (commaSeparated && spaceSeparated && !reported) {
                    reported = true; // do not spam in output, a single diag seems to be enough
                    reportDiagnostic(DiagnosticsId::DI_MIXED_SEPARATORS, "elements of array");
                }

                if (_token == _SC(',')) {
                    Lex();
                }
            }
            setCoordinates(arr, l, c);
            Lex();
            r = arr;
        }
        break;
    case _SC('{'): {
        Lex();
        TableDecl *t = newNode<TableDecl>(arena());
        _docObjectStack.push_back(&t->docObject);
        ParseTableOrClass(t, _SC(','), _SC('}'));
        _docObjectStack.pop_back();
        setCoordinates(t, l, c);
        r = setCoordinates(newNode<DeclExpr>(t), l, c);
        break;
    }
    case TK_FUNCTION:
        r = FunctionExp(false);
        break;
    case _SC('@'):
        r = FunctionExp(true);
        break;
    case TK_CLASS: {
        Lex();
        Decl *classDecl = ClassExp(NULL);
        r = setCoordinates(newNode<DeclExpr>(classDecl), l, c);
        break;
    }
    case _SC('-'):
        Lex();
        switch(_token) {
        case TK_INTEGER:
            r = setCoordinates(newNode<LiteralExpr>(-_lex._nvalue), l, c);
            Lex();
            break;
        case TK_FLOAT:
            r = setCoordinates(newNode<LiteralExpr>(-_lex._fvalue), l, c);
            Lex();
            break;
        default:
            r = setCoordinates(UnaryOP(TO_NEG), l, c);
            break;
        }
        break;
    case _SC('!'):
        Lex();
        r = setCoordinates(UnaryOP(TO_NOT), l, c);
        break;
    case _SC('~'):
        Lex();
        if(_token == TK_INTEGER)  {
            r = setCoordinates(newNode<LiteralExpr>(~_lex._nvalue), l, c);
            Lex();
        }
        else {
            r = setCoordinates(UnaryOP(TO_BNOT), l, c);
        }
        break;
    case TK_TYPEOF : Lex(); r = setCoordinates(UnaryOP(TO_TYPEOF), l, c); break;
    case TK_RESUME : Lex(); r = setCoordinates(UnaryOP(TO_RESUME), l, c); break;
    case TK_CLONE : Lex(); r = setCoordinates(UnaryOP(TO_CLONE), l, c); break;
    case TK_STATIC : Lex(); r = setCoordinates(UnaryOP(TO_STATIC_MEMO), l, c); break;
    case TK_CONST : Lex(); r = setCoordinates(UnaryOP(TO_INLINE_CONST), l, c); break;
    case TK_MINUSMINUS :
    case TK_PLUSPLUS :
        r = setCoordinates(PrefixIncDec(_token), l, c);
        break;
    case TK_DELETE :
        if (_lang_features & LF_FORBID_DELETE_OP) {
            reportDiagnostic(DiagnosticsId::DI_DELETE_OP_FORBIDDEN);
        }
        r = DeleteExpr();
        break;
    case _SC('('):
        Lex();
        r = setCoordinates(newNode<UnExpr>(TO_PAREN, Expression(_expression_context)), l, c);
        Expect(_SC(')'));
        break;
    case TK___LINE__:
        r = setCoordinates(newNode<LiteralExpr>(_lex._currentline), l, c);
        Lex();
        break;
    case TK___FILE__:
        r = setCoordinates(newNode<LiteralExpr>(_sourcename), l, c);
        Lex();
        break;
    default:
        reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "expression");
    }
    return r;
}

Expr* SQParser::UnaryOP(enum TreeOp op)
{
    NestingChecker nc(this);
    Expr *arg = PrefixedExpr();
    return newNode<UnExpr>(op, arg);
}

void SQParser::ParseTableOrClass(TableDecl *decl, SQInteger separator, SQInteger terminator)
{
    NestingChecker nc(this);
    NewObjectType otype = separator==_SC(',') ? NEWOBJ_TABLE : NEWOBJ_CLASS;

    while(_token != terminator) {
        SQInteger l = line();
        SQInteger c = column();
        unsigned flags = 0;
        //check if is an static
        if(otype == NEWOBJ_CLASS) {
            if(_token == TK_STATIC) {
                flags |= TMF_STATIC;
                Lex();
            }
        }

        switch (_token) {
        case TK_DOCSTRING: {
            onDocString(_lex._svalue);
            Lex();
            break;
        }
        case TK_FUNCTION:
        case TK_CONSTRUCTOR: {
            SQInteger tk = _token;
            SQInteger l = line(), c = column();
            Lex();
            FuncAttrFlagsType attr = ParseFunctionAttributes();
            Id *funcName = tk == TK_FUNCTION ? (Id *)Expect(TK_IDENTIFIER) : newNode<Id>(_SC("constructor"));
            assert(funcName);
            LiteralExpr *key = newNode<LiteralExpr>(funcName->name()); //-V522
            setCoordinates(key, l, c);
            Expect(_SC('('));
            FunctionDecl *f = CreateFunction(funcName, false, tk == TK_CONSTRUCTOR);
            f->setPure(attr & FATTR_PURE);
            DeclExpr *e = newNode<DeclExpr>(f);
            setCoordinates(e, l, c);
            decl->addMember(key, e, flags);
        }
        break;
        case _SC('['): {
            Lex();

            Expr *key = Expression(SQE_RVALUE); //-V522
            assert(key);
            Expect(_SC(']'));
            Expect(_SC('='));
            Expr *value = Expression(SQE_RVALUE);
            decl->addMember(key, value, flags | TMF_DYNAMIC_KEY);
            break;
        }
        case TK_STRING_LITERAL: //JSON
            if (otype == NEWOBJ_TABLE) { //only works for tables
                LiteralExpr *key = (LiteralExpr *)Expect(TK_STRING_LITERAL);  //-V522
                assert(key);
                Expect(_SC(':'));
                Expr *expr = Expression(SQE_RVALUE);
                decl->addMember(key, expr, flags | TMF_JSON);
                break;
            }  //-V796
        default: {
            Id *id = (Id *)setCoordinates(Expect(TK_IDENTIFIER), l, c);
            assert(id);
            LiteralExpr *key = newNode<LiteralExpr>(id->name()); //-V522
            copyCoordinates(id, key);
            if ((otype == NEWOBJ_TABLE) &&
                (_token == TK_IDENTIFIER || _token == separator || _token == terminator || _token == _SC('[')
                    || _token == TK_FUNCTION)) {
                decl->addMember(key, id, flags);
            }
            else {
                Expect(_SC('='));
                Expr *expr = Expression(SQE_RVALUE);
                decl->addMember(key, expr, flags);
            }
        }
        }
        if (_token == separator) Lex(); //optional comma/semicolon
    }

    Lex();
}

Decl *SQParser::parseLocalFunctionDeclStmt(bool assignable)
{
    SQInteger l = line(), c = column();

    assert(_token == TK_FUNCTION);
    Lex();

    FuncAttrFlagsType attr = ParseFunctionAttributes();
    Id *varname = (Id *)Expect(TK_IDENTIFIER);
    Expect(_SC('('));
    FunctionDecl *f = CreateFunction(varname, false);
    f->setPure(attr & FATTR_PURE);
    f->setLineStartPos(l); f->setColumnStartPos(c);
    VarDecl *d = newNode<VarDecl>(varname->name(), copyCoordinates(f, newNode<DeclExpr>(f)), assignable); //-V522
    setCoordinates(d, l, c);
    return d;
}

Decl *SQParser::parseLocalClassDeclStmt(bool assignable)
{
    SQInteger l = line(), c = column();

    assert(_token == TK_CLASS);
    Lex();

    Id *varname = (Id *)Expect(TK_IDENTIFIER);
    ClassDecl *cls = ClassExp(NULL);
    cls->setLineStartPos(l); cls->setColumnStartPos(c);
    VarDecl *d = newNode<VarDecl>(varname->name(), copyCoordinates(cls, newNode<DeclExpr>(cls)), assignable); //-V522
    setCoordinates(d, l, c);
    return d;
}

Decl* SQParser::parseLocalDeclStatement()
{
    NestingChecker nc(this);

    assert(_token == TK_LET || _token == TK_LOCAL);
    SQInteger gl = line(), gc = column();
    bool assignable = _token == TK_LOCAL;
    Lex();

    if (_token == TK_FUNCTION) {
        return parseLocalFunctionDeclStmt(assignable);
    } else if (_token == TK_CLASS) {
        return parseLocalClassDeclStmt(assignable);
    }


    DeclGroup *decls = NULL;
    DestructuringDecl  *dd = NULL;
    Decl *decl = NULL;
    SQInteger destructurer = 0;

    if (_token == _SC('{') || _token == _SC('[')) {
        SQInteger l = line(), c = column();
        destructurer = _token;
        Lex();
        decls = dd = newNode<DestructuringDecl>(arena(), destructurer == _SC('{') ? DT_TABLE : DT_ARRAY);
        setCoordinates(dd, l, c);
    }

    do {
        SQInteger l = line();
        SQInteger c = column();
        Id *varname = (Id *)setCoordinates(Expect(TK_IDENTIFIER), l, c);
        assert(varname);
        VarDecl *cur = NULL;
        if(_token == _SC('=')) {
            Lex();
            Expr *expr = Expression(SQE_REGULAR);
            if (!assignable && expr->op() == TO_DECL_EXPR && expr->asDeclExpr()->declaration()->op() == TO_FUNCTION) {
              FunctionDecl *f = static_cast<FunctionDecl *>(expr->asDeclExpr()->declaration());
              if (!f->name() || f->name()[0] == _SC('(')) {
                f->setName(varname->name());
              }
            }
            cur = newNode<VarDecl>(varname->name(), expr, assignable, destructurer != 0); //-V522
        }
        else {
            if (!assignable && !destructurer)
                _ctx.reportDiagnostic(DiagnosticsId::DI_UNINITIALIZED_BINDING, varname->lineStart(), varname->columnStart(), varname->textWidth(), varname->name()); //-V522
            cur = newNode<VarDecl>(varname->name(), nullptr, assignable, destructurer != 0);
        }

        setCoordinates(cur, l, c);

        if (decls) {
            decls->addDeclaration(cur);
        } else if (decl) {
            decls = setCoordinates(newNode<DeclGroup>(arena()), gl, gc);
            decls->addDeclaration(static_cast<VarDecl *>(decl));
            decls->addDeclaration(cur);
            decl = decls;
        } else {
            decl = cur;
        }

        if (destructurer) {
            if (_token == _SC(',')) {
                Lex();
                if (_token == _SC(']') || _token == _SC('}'))
                    break;
            }
            else if (_token == TK_IDENTIFIER)
                continue;
            else
                break;
        }
        else {
            if (_token == _SC(','))
                Lex();
            else
                break;
        }
    } while(1);

    if (destructurer) {
        Expect(destructurer==_SC('[') ? _SC(']') : _SC('}'));
        Expect(_SC('='));
        assert(dd);
        dd->setExpression(Expression(SQE_RVALUE)); //-V522
        return dd;
    } else {
        return decls ? static_cast<Decl*>(decls) : decl;
    }
}

Statement* SQParser::IfLikeBlock(bool &wrapped)
{
    NestingChecker nc(this);
    Statement *stmt = NULL;
    SQInteger l = line(), c = column();
    if (_token == _SC('{'))
    {
        wrapped = false;
        Lex();
        stmt = setCoordinates(parseStatements(), l, c);
        Expect(_SC('}'));
    }
    else {
        wrapped = true;
        stmt = parseStatement();
        Block *block = newNode<Block>(arena());
        block->addStatement(stmt);
        copyCoordinates(stmt, block);
        stmt = block;
        if (_lex._prevtoken != _SC('}') && _lex._prevtoken != _SC(';')) OptionalSemicolon();
    }

    return stmt;
}

IfStatement* SQParser::parseIfStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    SQInteger prevTok = _lex._prevtoken;

    Consume(TK_IF);

    Expect(_SC('('));
    Expr *cond = Expression(SQE_IF);
    Expect(_SC(')'));

    checkBraceIndentationStyle();

    bool wrapped = false;

    Statement *thenB = IfLikeBlock(wrapped);
    Statement *elseB = NULL;
    if (_token == TK_ELSE) {
        SQInteger el = line(), ec = column(), ew = width();
        Lex();
        if (_token != _SC('{') && prevTok != TK_ELSE && wrapped && l != el && c != ec) {
          _ctx.reportDiagnostic(DiagnosticsId::DI_SUSPICIOUS_FMT, el, ec, ew);
        }
        checkBraceIndentationStyle();
        elseB = IfLikeBlock(wrapped);
        if (!IsEndOfStatement()) {
          reportDiagnostic(DiagnosticsId::DI_STMT_SAME_LINE, "else");
        }
    }
    else if (!IsEndOfStatement()) {
      reportDiagnostic(DiagnosticsId::DI_STMT_SAME_LINE, "then");
    }


    if (_token != SQUIRREL_EOB && _token != _SC('}') && _token != _SC(']') && _token != _SC(')') && _token != _SC(',')) {
      SQInteger lastLine = elseB ? elseB->lineEnd() : thenB->lineEnd();
      if (line() != lastLine && column() > c) {
        reportDiagnostic(DiagnosticsId::DI_SUSPICIOUS_FMT);
      }
    }

    return setCoordinates(newNode<IfStatement>(cond, thenB, elseB), l, c);
}

WhileStatement* SQParser::parseWhileStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    Consume(TK_WHILE);

    Expect(_SC('('));
    Expr *cond = Expression(SQE_LOOP_CONDITION);
    Expect(_SC(')'));

    bool wrapped = false;
    checkBraceIndentationStyle();
    Statement *body = IfLikeBlock(wrapped);

    if (!IsEndOfStatement()) {
      reportDiagnostic(DiagnosticsId::DI_STMT_SAME_LINE, "while loop body");
    }

    if (_token != SQUIRREL_EOB && _token != _SC('}')) {
      if (line() != body->lineEnd() && column() > c) {
        reportDiagnostic(DiagnosticsId::DI_SUSPICIOUS_FMT);
      }
    }

    return setCoordinates(newNode<WhileStatement>(cond, body), l, c);
}

DoWhileStatement* SQParser::parseDoWhileStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    Consume(TK_DO); // DO

    bool wrapped = false;
    checkBraceIndentationStyle();
    Statement *body = IfLikeBlock(wrapped);

    Expect(TK_WHILE);

    Expect(_SC('('));
    Expr *cond = Expression(SQE_LOOP_CONDITION);
    Expect(_SC(')'));

    return setCoordinates(newNode<DoWhileStatement>(body, cond), l, c);
}

ForStatement* SQParser::parseForStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    Consume(TK_FOR);

    Expect(_SC('('));

    Node *init = NULL;
    if (_token == TK_LOCAL)
        init = parseLocalDeclStatement();
    else if (_token != _SC(';')) {
        init = parseCommaExpr(SQE_REGULAR);
    }
    Expect(_SC(';'));

    Expr *cond = NULL;
    if(_token != _SC(';')) {
        cond = Expression(SQE_LOOP_CONDITION);
    }
    Expect(_SC(';'));

    Expr *mod = NULL;
    if(_token != _SC(')')) {
        mod = parseCommaExpr(SQE_REGULAR);
    }
    Expect(_SC(')'));

    bool wrapped = false;
    checkBraceIndentationStyle();
    Statement *body = IfLikeBlock(wrapped);

    if (!IsEndOfStatement()) {
      reportDiagnostic(DiagnosticsId::DI_STMT_SAME_LINE, "for loop body");
    }

    if (_token != SQUIRREL_EOB && _token != _SC('}')) {
      if (line() != body->lineEnd() && column() > c) {
        reportDiagnostic(DiagnosticsId::DI_SUSPICIOUS_FMT);
      }
    }

    return setCoordinates(newNode<ForStatement>(init, cond, mod, body), l, c);
}

ForeachStatement* SQParser::parseForEachStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    Consume(TK_FOREACH);

    Expect(_SC('('));
    SQInteger idL = line(), idC = column();

    Id *valname = (Id *)setCoordinates(Expect(TK_IDENTIFIER), idL, idC);
    assert(valname);

    Id *idxname = NULL;
    if(_token == _SC(',')) {
        idxname = valname;
        Lex();
        idL = line();
        idC = column();
        valname = (Id *)setCoordinates(Expect(TK_IDENTIFIER), idL, idC);
        assert(valname);

        if (strcmp(idxname->name(), valname->name()) == 0) //-V522
            _ctx.reportDiagnostic(DiagnosticsId::DI_SAME_FOREACH_KV_NAMES, valname->lineStart(), valname->columnStart(), valname->textWidth(), valname->name());
    }
    else {
        //idxname = newNode<Id>(_SC("@INDEX@"));
    }

    Expect(TK_IN);

    Expr *contnr = Expression(SQE_RVALUE);
    Expect(_SC(')'));

    bool wrapped = false;
    checkBraceIndentationStyle();
    Statement *body = IfLikeBlock(wrapped);

    if (!IsEndOfStatement()) {
      reportDiagnostic(DiagnosticsId::DI_STMT_SAME_LINE, "foreach loop body");
    }

    if (_token != SQUIRREL_EOB && _token != _SC('}')) {
      if (line() != body->lineEnd() && column() > c) {
        reportDiagnostic(DiagnosticsId::DI_SUSPICIOUS_FMT);
      }
    }

    VarDecl *idxDecl = idxname ? copyCoordinates(idxname, newNode<VarDecl>(idxname->name(), nullptr, false)) : NULL;
    VarDecl *valDecl = valname ? copyCoordinates(valname, newNode<VarDecl>(valname->name(), nullptr, false)) : NULL;

    return setCoordinates(newNode<ForeachStatement>(idxDecl, valDecl, contnr, body), l, c);
}

SwitchStatement* SQParser::parseSwitchStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    Consume(TK_SWITCH);

    Expect(_SC('('));
    Expr *switchExpr = Expression(SQE_SWITCH);
    Expect(_SC(')'));

    checkBraceIndentationStyle();
    Expect(_SC('{'));

    SwitchStatement *switchStmt = newNode<SwitchStatement>(arena(), switchExpr);

    while(_token == TK_CASE) {
        Consume(TK_CASE);

        Expr *cond = Expression(SQE_RVALUE);
        Expect(_SC(':'));

        checkBraceIndentationStyle();
        Statement *caseBody = parseStatements();
        switchStmt->addCases(cond, caseBody);
    }

    if(_token == TK_DEFAULT) {
        Consume(TK_DEFAULT);
        Expect(_SC(':'));

        checkBraceIndentationStyle();
        switchStmt->addDefault(parseStatements());
    }

    Expect(_SC('}'));

    return setCoordinates(switchStmt, l, c);
}


LiteralExpr* SQParser::ExpectScalar()
{
    NestingChecker nc(this);
    LiteralExpr *ret = NULL;
    SQInteger l = line(), c = column();

    switch(_token) {
        case TK_NULL:
            ret = newNode<LiteralExpr>();
            break;
        case TK_INTEGER:
            ret = newNode<LiteralExpr>(_lex._nvalue);
            break;
        case TK_FLOAT:
            ret = newNode<LiteralExpr>(_lex._fvalue);
            break;
        case TK_STRING_LITERAL:
            ret = newStringLiteral(_lex._svalue);
            break;
        case TK_TRUE:
        case TK_FALSE:
            ret = newNode<LiteralExpr>((bool)(_token == TK_TRUE ? 1 : 0));
            break;
        case '-':
            Lex();
            switch(_token)
            {
            case TK_INTEGER:
                ret = newNode<LiteralExpr>(-_lex._nvalue);
            break;
            case TK_FLOAT:
                ret = newNode<LiteralExpr>(-_lex._fvalue);
            break;
            default:
                reportDiagnostic(DiagnosticsId::DI_SCALAR_EXPECTED, "integer, float");
            }
            break;
        default:
            reportDiagnostic(DiagnosticsId::DI_SCALAR_EXPECTED, "integer, float, or string");
    }

    setCoordinates(ret, l, c);

    Lex();
    return ret;
}


ConstDecl* SQParser::parseConstStatement(bool global)
{
    NestingChecker nc(this);
    SQInteger l = line(), c = column();
    Lex();
    Id *id = (Id *)Expect(TK_IDENTIFIER);

    Expect('=');
    Expr *valExpr = Expression(SQE_RVALUE);
    ConstDecl *d = setCoordinates(newNode<ConstDecl>(id->name(), valExpr, global), l, c); //-V522

    OptionalSemicolon();

    return d;
}


EnumDecl* SQParser::parseEnumStatement(bool global)
{
    NestingChecker nc(this);
    SQInteger l = line(), c = column();
    Lex();
    Id *id = (Id *)Expect(TK_IDENTIFIER);

    EnumDecl *decl = newNode<EnumDecl>(arena(), id->name(), global); //-V522

    checkBraceIndentationStyle();
    Expect(_SC('{'));

    SQInteger nval = 0;
    while(_token != _SC('}')) {
        SQInteger cl = line(), cc = column();
        Id *key = (Id *)setCoordinates(Expect(TK_IDENTIFIER), cl, cc);

        const char* keyName = key->name();
        for (EnumConst& ec : decl->consts())
            if (*keyName == *ec.id && !strcmp(keyName, ec.id))
                reportDiagnostic(DiagnosticsId::DI_DUPLICATE_KEY, keyName);

        LiteralExpr *valExpr = NULL;
        if(_token == _SC('=')) {
            // TODO1: should it behave like C does?
            // TODO2: should float and string literal be allowed here?
            Lex();
            valExpr = ExpectScalar();
        }
        else {
            valExpr = newNode<LiteralExpr>(nval++);
            copyCoordinates(key, valExpr);
        }

        decl->addConst(keyName, valExpr); //-V522

        if(_token == ',') Lex();
    }

    setCoordinates(decl, l, c);
    Lex();

    return decl;
}


TryStatement* SQParser::parseTryCatchStatement()
{
    NestingChecker nc(this);

    SQInteger l = line(), c = column();

    Consume(TK_TRY);

    checkBraceIndentationStyle();
    Statement *t = parseStatement();

    Expect(TK_CATCH);

    Expect(_SC('('));
    Id *exid = (Id *)Expect(TK_IDENTIFIER);
    Expect(_SC(')'));

    checkBraceIndentationStyle();
    Statement *cth = parseStatement();

    return setCoordinates(newNode<TryStatement>(t, exid, cth), l, c);
}


Id* SQParser::generateSurrogateFunctionName()
{
    const SQChar * fileName = _sourcename ? _sourcename : _SC("unknown");
    int lineNum = int(_lex._currentline);

    const SQChar * rightSlash = std::max(strrchr(fileName, _SC('/')), strrchr(fileName, _SC('\\')));

    constexpr int maxLen = 256;

    SQChar buf[maxLen];
    scsprintf(buf, maxLen, _SC("(%s:%d)"), rightSlash ? (rightSlash + 1) : fileName, lineNum);
    return newId(buf);
}


SQParser::FuncAttrFlagsType SQParser::ParseFunctionAttributes() {
    if (_token != '[')
        return 0;

    Lex();

    FuncAttrFlagsType attrVal = 0;
    while (_token != ']') {
        if (_token == TK_IDENTIFIER) {
            FuncAttrFlagsType flag = 0;
            if (strcmp(_lex._svalue, "pure")==0) {
                flag = FATTR_PURE;
            } else {
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "valid attribute name");
            }
            if (attrVal & flag) {
                reportDiagnostic(DiagnosticsId::DI_DUPLICATE_FUNC_ATTR, _lex._svalue);
            }
            attrVal |= flag;
        } else {
            reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "attribute name");
        }

        Lex();
        if (_token == _SC(',')) {
            Lex();
        }
    }

    Expect(']');
    return attrVal;
}


DeclExpr* SQParser::FunctionExp(bool lambda)
{
    NestingChecker nc(this);
    SQInteger l = line(), c = column();
    Lex();
    FuncAttrFlagsType attr = ParseFunctionAttributes();
    Id *funcName = (_token == TK_IDENTIFIER) ? (Id *)Expect(TK_IDENTIFIER) : generateSurrogateFunctionName();
    Expect(_SC('('));

    Decl *f = CreateFunction(funcName, lambda);
    static_cast<FunctionDecl *>(f)->setPure(attr & FATTR_PURE);
    f->setLineStartPos(l);
    f->setColumnStartPos(c);

    return setCoordinates(newNode<DeclExpr>(f), l, c);
}


ClassDecl* SQParser::ClassExp(Expr *key)
{
    NestingChecker nc(this);
    SQInteger l = line(), c = column();
    Expr *baseExpr = NULL;
    if(_token == TK_EXTENDS) {
        Lex();
        baseExpr = Expression(SQE_RVALUE);
    }
    else if (_token == _SC('(')) {
      Lex();
      baseExpr = Expression(SQE_RVALUE);
      Expect(_SC(')'));
    }
    checkBraceIndentationStyle();
    Expect(_SC('{'));
    ClassDecl *d = newNode<ClassDecl>(arena(), key, baseExpr);
    _docObjectStack.push_back(&d->docObject);
    ParseTableOrClass(d, _SC(';'),_SC('}'));
    _docObjectStack.pop_back();
    return setCoordinates(d, l, c);
}


Expr* SQParser::DeleteExpr()
{
    NestingChecker nc(this);
    SQInteger l = line(), c = column();
    Consume(TK_DELETE);
    Expr *arg = PrefixedExpr();
    Expr *d = newNode<UnExpr>(TO_DELETE, arg);
    d->setLineStartPos(l);
    d->setColumnStartPos(c);
    return d;
}


Expr* SQParser::PrefixIncDec(SQInteger token)
{
    NestingChecker nc(this);
    SQInteger diff = (token==TK_MINUSMINUS) ? -1 : 1;
    Lex();
    Expr *arg = PrefixedExpr();
    return newNode<IncExpr>(arg, diff, IF_PREFIX);
}


FunctionDecl* SQParser::CreateFunction(Id *name, bool lambda, bool ctor)
{
    NestingChecker nc(this);
    FunctionDecl *f = ctor ? newNode<ConstructorDecl>(arena(), name->name()) : newNode<FunctionDecl>(arena(), name->name());
    f->setLineStartPos(line()); f->setColumnStartPos(column());

    SQInteger defparams = 0;

    auto &params = f->parameters();

    while (_token!=_SC(')')) {
        SQInteger l = line(), c = column();
        if (_token == TK_VARPARAMS) {
            if(defparams > 0)
                reportDiagnostic(DiagnosticsId::DI_VARARG_WITH_DEFAULT_ARG);
            f->addParameter(_SC("vargv"));
            f->setVararg(true);
            params.back()->setVararg();
            Lex();
            if(_token != _SC(')'))
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, ")");
            break;
        }
        else {
            Id *paramname = (Id *)Expect(TK_IDENTIFIER);
            Expr *defVal = NULL;
            if (_token == _SC('=')) {
                Lex();
                defVal = Expression(SQE_RVALUE);
                defparams++;
            }
            else {
                if (defparams > 0)
                    reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "=");
            }

            f->addParameter(paramname->name(), defVal); //-V522
            ParamDecl *p = params.back();
            p->setLineStartPos(l); p->setColumnStartPos(c);
            p->setLineEndPos(_lex._currentline); p->setColumnEndPos(_lex._currentcolumn - 1);

            if(_token == _SC(',')) Lex();
            else if(_token != _SC(')'))
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, ") or ,");
        }
    }

    Expect(_SC(')'));

    Block *body = NULL;

    SQUnsignedInteger savedLangFeatures = _lang_features;
    _docObjectStack.push_back(&f->docObject);

    if (lambda) {
        SQInteger line = _lex._prevtoken == _SC('\n') ? _lex._lasttokenline : _lex._currentline;
        Expr *expr = Expression(SQE_REGULAR);
        ReturnStatement *retStmt = newNode<ReturnStatement>(expr);
        copyCoordinates(expr, retStmt);
        retStmt->setIsLambda();
        body = newNode<Block>(arena());
        body->addStatement(retStmt);
        copyCoordinates(retStmt, body);
    }
    else {
        if (_token != '{')
            reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "{");
        checkBraceIndentationStyle();
        body = (Block *)parseStatement(false);
    }

    _docObjectStack.pop_back();
    SQInteger line2 = _lex._prevtoken == _SC('\n') ? _lex._lasttokenline : _lex._currentline;
    SQInteger column2 = _lex._prevtoken == _SC('\n') ? _lex._lasttokencolumn : _lex._currentcolumn;

    f->setBody(body);

    f->setSourceName(_sourcename);
    f->setLambda(lambda);

    f->setLineEndPos(line2);
    f->setColumnEndPos(column2);

    _lang_features = savedLangFeatures;

    return f;
}


ImportStmt* SQParser::parseImportStatement()
{
    if (!_are_imports_still_allowed)
        reportDiagnostic(DiagnosticsId::DI_GENERAL_COMPILE_ERROR, "import statements must be at the top of the file");

    // Reuse lexer for the specific import syntax

    SQInteger l = line(), c = column();
    ImportStmt *importStmt = nullptr;

    const char *keyword = _lex._svalue;

    if (strcmp(keyword, "from") == 0) {
        // from "module" import x, y, z
        Lex();

        if (_token != TK_STRING_LITERAL)
            reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "module name");

        const char *moduleName = copyString(_lex._svalue);
        Lex();

        if (_token != TK_IDENTIFIER || strcmp(_lex._svalue, "import") != 0)
            reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "import");
        Lex();

        importStmt = newNode<ImportStmt>(arena(), moduleName, nullptr);

        // Parse import list: x, y, z as foo, *
        do {
            SQModuleImportSlot slot{};

            if (_token != '*' && _token != TK_IDENTIFIER)
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "identifier or *");

            bool isWildcard = (_token == '*');

            slot.name = copyString(isWildcard ? "*" : _lex._svalue);
            Lex();

            // Check for an alias
            if (_token == TK_IDENTIFIER && strcmp(_lex._svalue, "as") == 0) {
                Lex();
                if (_token != TK_IDENTIFIER)
                    reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "identifier");
                if (isWildcard)
                    reportDiagnostic(DiagnosticsId::DI_GENERAL_COMPILE_ERROR, "cannot rename *");
                slot.alias = copyString(_lex._svalue);
                Lex();
            }

            slot.line = line();
            slot.column = column();
            importStmt->slots.push_back(slot);

            if (_token == ',')
                Lex();
            else
                break;
        } while (true);

    } else if (strcmp(keyword, "import") == 0) {
        // import "module" [as alias]
        Lex();

        if (_token != TK_STRING_LITERAL)
            reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "module name string");

        const char *moduleName = copyString(_lex._svalue);
        Lex();

        const char *moduleAlias = nullptr;
        if (_token == TK_IDENTIFIER && strcmp(_lex._svalue, "as") == 0) {
            Lex();
            if (_token != TK_IDENTIFIER)
                reportDiagnostic(DiagnosticsId::DI_EXPECTED_TOKEN, "identifier");
            moduleAlias = copyString(_lex._svalue);
            Lex();
        }

        importStmt = newNode<ImportStmt>(arena(), moduleName, moduleAlias);
    }

    if (importStmt) {
        setCoordinates(importStmt, l, c);
    }

    return importStmt;
}

} // namespace SQCompilation

#endif
