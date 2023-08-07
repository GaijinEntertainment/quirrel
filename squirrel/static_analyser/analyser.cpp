#include "analyser.h"
#include <stdarg.h>

namespace SQCompilation {

const Expr *deparen(const Expr *e) {
  if (!e) return nullptr;

  if (e->op() == TO_PAREN)
    return deparen(static_cast<const UnExpr *>(e)->argument());
  return e;
}

const Expr *skipUnary(const Expr *e) {
  if (!e) return nullptr;

  if (e->op() == TO_INC) {
    return skipUnary(static_cast<const IncExpr *>(e)->argument());
  }

  if (TO_NOT <= e->op() && e->op() <= TO_DELETE) {
    return skipUnary(static_cast<const UnExpr *>(e)->argument());
  }

  return e;
}

const Statement *unwrapBody(Statement *stmt) {

  if (stmt == nullptr)
    return stmt;

  if (stmt->op() != TO_BLOCK)
    return stmt;

  auto &stmts = stmt->asBlock()->statements();

  if (stmts.size() != 1)
    return nullptr;

  return unwrapBody(stmts[0]);
}

static Expr *unwrapExprStatement(Statement *stmt) {
  return stmt->op() == TO_EXPR_STMT ? static_cast<ExprStatement *>(stmt)->expression() : nullptr;
}

StaticAnalyser::StaticAnalyser(SQCompilationContext &ctx)
  : _ctx(ctx) {

}

class NodeEqualChecker {

  template<typename N>
  bool cmpNodeVector(const ArenaVector<N *> &l, const ArenaVector<N *> &r) const {
    if (l.size() != r.size())
      return false;

    for (int32_t i = 0; i < l.size(); ++i) {
      if (!check(l[i], r[i]))
        return false;
    }

    return true;
  }

  bool cmpId(const Id *l, const Id* r) const {
    return strcmp(l->id(), r->id()) == 0;
  }

  bool cmpLiterals(const LiteralExpr *l, const LiteralExpr *r) const {
    if (l->kind() != r->kind())
      return false;

    switch (l->kind())
    {
    case LK_STRING: return strcmp(l->s(), r->s()) == 0;
    default: return l->raw() == r->raw();
    }
  }

  bool cmpBinary(const BinExpr *l, const BinExpr *r) const {
    return check(l->lhs(), r->lhs()) && check(l->rhs(), r->rhs());
  }

  bool cmpUnary(const UnExpr *l, const UnExpr *r) const {
    return check(l->argument(), r->argument());
  }

  bool cmpTernary(const TerExpr *l, const TerExpr *r) const {
    return check(l->a(), r->a()) && check(l->b(), r->b()) && check(l->c(), r->c());
  }

  bool cmpBlock(const Block *l, const Block *r) const {
    if (l->isRoot() != r->isRoot())
      return false;

    if (l->isBody() != r->isBody())
      return false;

    return cmpNodeVector(l->statements(), r->statements());
  }

  bool cmpIf(const IfStatement *l, const IfStatement *r) const {
    if (!check(l->condition(), r->condition()))
      return false;

    if (!check(l->thenBranch(), r->thenBranch()))
      return false;

    return check(l->elseBranch(), r->elseBranch());
  }

  bool cmpWhile(const WhileStatement *l, const WhileStatement *r) const {
    if (!check(l->condition(), r->condition()))
      return false;

    return check(l->body(), r->body());
  }

  bool cmpDoWhile(const DoWhileStatement *l, const DoWhileStatement *r) const {
    if (!check(l->body(), r->body()))
      return false;

    return check(l->condition(), r->condition());
  }

  bool cmpFor(const ForStatement *l, const ForStatement *r) const {
    if (!check(l->initializer(), r->initializer()))
      return false;

    if (!check(l->condition(), r->condition()))
      return false;

    if (!check(l->modifier(), r->modifier()))
      return false;

    return check(l->body(), r->body());
  }

  bool cmpForeach(const ForeachStatement *l, const ForeachStatement *r) const {
    if (!check(l->idx(), r->idx()))
      return false;

    if (!check(l->val(), r->val()))
      return false;

    if (!check(l->container(), r->container()))
      return false;

    return check(l->body(), r->body());
  }

  bool cmpSwitch(const SwitchStatement *l, const SwitchStatement *r) const {
    if (!check(l->expression(), r->expression()))
      return false;

    const auto &lcases = l->cases();
    const auto &rcases = r->cases();

    if (lcases.size() != rcases.size())
      return false;

    for (int32_t i = 0; i < lcases.size(); ++i) {
      const auto &lc = lcases[i];
      const auto &rc = rcases[i];

      if (!check(lc.val, rc.val))
        return false;

      if (!check(lc.stmt, rc.stmt))
        return false;
    }

    return check(l->defaultCase().stmt, r->defaultCase().stmt);
  }

  bool cmpTry(const TryStatement *l, const TryStatement *r) const {
    if (!check(l->tryStatement(), r->tryStatement()))
      return false;

    if (!check(l->exceptionId(), r->exceptionId()))
      return false;

    return check(l->catchStatement(), r->catchStatement());
  }

  bool cmpTerminate(const TerminateStatement *l, const TerminateStatement *r) const {
    return check(l->argument(), r->argument());
  }

  bool cmpReturn(const ReturnStatement *l, const ReturnStatement *r) const {
    return l->isLambdaReturn() == r->isLambdaReturn() && cmpTerminate(l, r);
  }

  bool cmpExprStmt(const ExprStatement *l, const ExprStatement *r) const {
    return check(l->expression(), r->expression());
  }

  bool cmpComma(const CommaExpr *l, const CommaExpr *r) const {
    return cmpNodeVector(l->expressions(), r->expressions());
  }

  bool cmpIncExpr(const IncExpr *l, const IncExpr *r) const {
    if (l->form() != r->form())
      return false;

    if (l->diff() != r->diff())
      return false;

    return check(l->argument(), r->argument());
  }

  bool cmpDeclExpr(const DeclExpr *l, const DeclExpr *r) const {
    return check(l->declaration(), r->declaration());
  }

  bool cmpCallExpr(const CallExpr *l, const CallExpr *r) const {
    if (l->isNullable() != r->isNullable())
      return false;

    if (!check(l->callee(), r->callee()))
      return false;

    return cmpNodeVector(l->arguments(), r->arguments());
  }

  bool cmpArrayExpr(const ArrayExpr *l, const ArrayExpr *r) const {
    return cmpNodeVector(l->initialziers(), r->initialziers());
  }

  bool cmpGetField(const GetFieldExpr *l, const GetFieldExpr *r) const {
    if (l->isNullable() != r->isNullable())
      return false;

    if (strcmp(l->fieldName(), r->fieldName()))
      return false;

    return check(l->receiver(), r->receiver());
  }

  bool cmpGetTable(const GetTableExpr *l, const GetTableExpr *r) const {
    if (l->isNullable() != r->isNullable())
      return false;

    if (!check(l->key(), r->key()))
      return false;

    return check(l->receiver(), r->receiver());
  }

  bool cmpValueDecl(const ValueDecl *l, const ValueDecl *r) const {
    if (!check(l->expression(), r->expression()))
      return false;

    return strcmp(l->name(), r->name()) == 0;
  }

  bool cmpVarDecl(const VarDecl *l, const VarDecl *r) const {
    return l->isAssignable() == r->isAssignable() && cmpValueDecl(l, r);
  }

  bool cmpConst(const ConstDecl *l, const ConstDecl *r) const {
    if (l->isGlobal() != r->isGlobal())
      return false;

    if (strcmp(l->name(), r->name()))
      return false;

    return cmpLiterals(l->value(), r->value());
  }

  bool cmpDeclGroup(const DeclGroup *l, const DeclGroup *r) const {
    return cmpNodeVector(l->declarations(), r->declarations());
  }

  bool cmpDestructDecl(const DestructuringDecl *l, const DestructuringDecl *r) const {
    return l->type() == r->type() && check(l->initiExpression(), r->initiExpression());
  }

  bool cmpFunction(const FunctionDecl *l, const FunctionDecl *r) const {
    if (l->isVararg() != r->isVararg())
      return false;

    if (l->isLambda() != r->isLambda())
      return false;

    if (strcmp(l->name(), r->name()))
      return false;

    if (!cmpNodeVector(l->parameters(), r->parameters()))
      return false;

    return check(l->body(), r->body());
  }

  bool cmpTable(const TableDecl *l, const TableDecl *r) const {
    const auto &lmems = l->members();
    const auto &rmems = r->members();

    if (lmems.size() != rmems.size())
      return false;

    for (int32_t i = 0; i < lmems.size(); ++i) {
      const auto &lm = lmems[i];
      const auto &rm = rmems[i];

      if (!check(lm.key, rm.key))
        return false;

      if (!check(lm.value, rm.value))
        return false;

      if (lm.isStatic != rm.isStatic)
        return false;
    }

    return true;
  }

  bool cmpClass(const ClassDecl *l, const ClassDecl *r) const {
    if (!check(l->classBase(), r->classBase()))
      return false;

    if (!check(l->classKey(), r->classKey()))
      return false;

    return cmpTable(l, r);
  }

  bool cmpEnumDecl(const EnumDecl *l, const EnumDecl *r) const {
    if (l->isGlobal() != r->isGlobal())
      return false;

    if (strcmp(l->name(), r->name()))
      return false;

    const auto &lcs = l->consts();
    const auto &rcs = r->consts();

    if (lcs.size() != rcs.size())
      return false;

    for (int32_t i = 0; i < lcs.size(); ++i) {
      const auto &lc = lcs[i];
      const auto &rc = rcs[i];

      if (strcmp(lc.id, lc.id))
        return false;

      if (!cmpLiterals(lc.val, rc.val))
        return false;
    }

    return true;
  }

public:

  bool check(const Node *lhs, const Node *rhs) const {

    if (lhs == rhs)
      return true;

    if (!lhs || !rhs)
      return false;

    if (lhs->op() != rhs->op())
      return false;

    switch (lhs->op())
    {
    case TO_BLOCK:      return cmpBlock((const Block *)lhs, (const Block *)rhs);
    case TO_IF:         return cmpIf((const IfStatement *)lhs, (const IfStatement *)rhs);
    case TO_WHILE:      return cmpWhile((const WhileStatement *)lhs, (const WhileStatement *)rhs);
    case TO_DOWHILE:    return cmpDoWhile((const DoWhileStatement *)lhs, (const DoWhileStatement *)rhs);
    case TO_FOR:        return cmpFor((const ForStatement *)lhs, (const ForStatement *)rhs);
    case TO_FOREACH:    return cmpForeach((const ForeachStatement *)lhs, (const ForeachStatement *)rhs);
    case TO_SWITCH:     return cmpSwitch((const SwitchStatement *)lhs, (const SwitchStatement *)rhs);
    case TO_RETURN:
      return cmpReturn((const ReturnStatement *)lhs, (const ReturnStatement *)rhs);
    case TO_YIELD:
    case TO_THROW:
      return cmpTerminate((const TerminateStatement *)lhs, (const TerminateStatement *)rhs);
    case TO_TRY:
      return cmpTry((const TryStatement *)lhs, (const TryStatement *)rhs);
    case TO_BREAK:
    case TO_CONTINUE:
    case TO_EMPTY:
      return true;
    case TO_EXPR_STMT:
      return cmpExprStmt((const ExprStatement *)lhs, (const ExprStatement *)rhs);

      //case TO_STATEMENT_MARK:
    case TO_ID:         return cmpId((const Id *)lhs, (const Id *)rhs);
    case TO_COMMA:      return cmpComma((const CommaExpr *)lhs, (const CommaExpr *)rhs);
    case TO_NULLC:
    case TO_ASSIGN:
    case TO_OROR:
    case TO_ANDAND:
    case TO_OR:
    case TO_XOR:
    case TO_AND:
    case TO_NE:
    case TO_EQ:
    case TO_3CMP:
    case TO_GE:
    case TO_GT:
    case TO_LE:
    case TO_LT:
    case TO_IN:
    case TO_INSTANCEOF:
    case TO_USHR:
    case TO_SHR:
    case TO_SHL:
    case TO_MUL:
    case TO_DIV:
    case TO_MOD:
    case TO_ADD:
    case TO_SUB:
    case TO_NEWSLOT:
    case TO_INEXPR_ASSIGN:
    case TO_PLUSEQ:
    case TO_MINUSEQ:
    case TO_MULEQ:
    case TO_DIVEQ:
    case TO_MODEQ:
      return cmpBinary((const BinExpr *)lhs, (const BinExpr *)rhs);
    case TO_NOT:
    case TO_BNOT:
    case TO_NEG:
    case TO_TYPEOF:
    case TO_RESUME:
    case TO_CLONE:
    case TO_PAREN:
    case TO_DELETE:
      return cmpUnary((const UnExpr *)lhs, (const UnExpr *)rhs);
    case TO_LITERAL:
      return cmpLiterals((const LiteralExpr *)lhs, (const LiteralExpr *)rhs);
    case TO_BASE:
    case TO_ROOT:
      return true;
    case TO_INC:
      return cmpIncExpr((const IncExpr *)lhs, (const IncExpr *)rhs);
    case TO_DECL_EXPR:
      return cmpDeclExpr((const DeclExpr *)lhs, (const DeclExpr *)rhs);
    case TO_ARRAYEXPR:
      return cmpArrayExpr((const ArrayExpr *)lhs, (const ArrayExpr *)rhs);
    case TO_GETFIELD:
      return cmpGetField((const GetFieldExpr *)lhs, (const GetFieldExpr *)rhs);
    case TO_SETFIELD:
      assert(0); return false;
    case TO_GETTABLE:
      return cmpGetTable((const GetTableExpr *)lhs, (const GetTableExpr *)rhs);
    case TO_SETTABLE:
      assert(0); return false;
    case TO_CALL:
      return cmpCallExpr((const CallExpr *)lhs, (const CallExpr *)rhs);
    case TO_TERNARY:
      return cmpTernary((const TerExpr *)lhs, (const TerExpr *)rhs);
      //case TO_EXPR_MARK:
    case TO_VAR:
      return cmpVarDecl((const VarDecl *)lhs, (const VarDecl *)rhs);
    case TO_PARAM:
      return cmpValueDecl((const ValueDecl *)lhs, (const ValueDecl *)rhs);
    case TO_CONST:
      return cmpConst((const ConstDecl *)lhs, (const ConstDecl *)rhs);
    case TO_DECL_GROUP:
      return cmpDeclGroup((const DeclGroup *)lhs, (const DeclGroup *)rhs);
    case TO_DESTRUCT:
      return cmpDestructDecl((const DestructuringDecl *)lhs, (const DestructuringDecl *)rhs);
    case TO_FUNCTION:
    case TO_CONSTRUCTOR:
      return cmpFunction((const FunctionDecl *)lhs, (const FunctionDecl *)rhs);
    case TO_CLASS:
      return cmpClass((const ClassDecl *)lhs, (const ClassDecl *)rhs);
    case TO_ENUM:
      return cmpEnumDecl((const EnumDecl *)lhs, (const EnumDecl *)rhs);
    case TO_TABLE:
      return cmpTable((const TableDecl *)lhs, (const TableDecl *)rhs);
    default:
      assert(0);
      return false;
    }
  }
};

enum ReturnTypeBits
{
  RT_NOTHING = 1 << 0,
  RT_NULL = 1 << 1,
  RT_BOOL = 1 << 2,
  RT_NUMBER = 1 << 3,
  RT_STRING = 1 << 4,
  RT_TABLE = 1 << 5,
  RT_ARRAY = 1 << 6,
  RT_CLOSURE = 1 << 7,
  RT_FUNCTION_CALL = 1 << 8,
  RT_UNRECOGNIZED = 1 << 9,
  RT_THROW = 1 << 10,
  RT_CLASS = 1 << 11,
};

class FunctionReturnTypeEvaluator {

  void checkLiteral(const LiteralExpr *l);
  void checkDeclaration(const DeclExpr *de);

  bool checkNode(const Statement *node);

  bool checkBlock(const Block *b);
  bool checkIf(const IfStatement *stmt);
  bool checkLoop(const LoopStatement *loop);
  bool checkSwitch(const SwitchStatement *swtch);
  bool checkReturn(const ReturnStatement *ret);
  bool checkTry(const TryStatement *trstmt);
  bool checkThrow(const ThrowStatement *thrw);

public:

  unsigned flags;

  unsigned compute(const Statement *n) {
    flags = 0;
    bool r = checkNode(n);
    if (!r)
      flags |= RT_NOTHING;

    return flags;
  }
};

bool FunctionReturnTypeEvaluator::checkNode(const Statement *n) {
  switch (n->op())
  {
  case TO_RETURN: return checkReturn(static_cast<const ReturnStatement *>(n));
  case TO_THROW: return checkThrow(static_cast<const ThrowStatement *>(n));
  case TO_FOR: case TO_FOREACH: case TO_WHILE: case TO_DOWHILE:
    return checkLoop(static_cast<const LoopStatement *>(n));
  case TO_IF: return checkIf(static_cast<const IfStatement *>(n));
  case TO_SWITCH: return checkSwitch(static_cast<const SwitchStatement *>(n));
  case TO_BLOCK: return checkBlock(static_cast<const Block *>(n));
  case TO_TRY: return checkTry(static_cast<const TryStatement *>(n));
  default:
    return false;
  }
}

void FunctionReturnTypeEvaluator::checkLiteral(const LiteralExpr *lit) {
  switch (lit->kind())
  {
  case LK_STRING: flags |= RT_STRING; break;
  case LK_NULL: flags |= RT_NULL; break;
  case LK_BOOL: flags |= RT_BOOL; break;
  default: flags |= RT_NUMBER; break;
  }
}

void FunctionReturnTypeEvaluator::checkDeclaration(const DeclExpr *de) {
  const Decl *decl = de->declaration();

  switch (decl->op())
  {
  case TO_CLASS: flags |= RT_CLASS; break;
  case TO_FUNCTION: flags |= RT_CLOSURE; break;
  case TO_TABLE: flags |= RT_TABLE; break;
  default:
    break;
  }
}

bool FunctionReturnTypeEvaluator::checkReturn(const ReturnStatement *ret) {

  const Expr *arg = deparen(ret->argument());

  if (arg == nullptr) {
    flags |= RT_NOTHING;
    return true;
  }

  switch (arg->op())
  {
  case TO_LITERAL:
    checkLiteral(static_cast<const LiteralExpr *>(arg));
    break;
  case TO_OROR:
  case TO_ANDAND:
  case TO_NE:
  case TO_EQ:
  case TO_GE:
  case TO_GT:
  case TO_LE:
  case TO_LT:
  case TO_INSTANCEOF:
  case TO_IN:
  case TO_NOT:
    flags |= RT_BOOL;
    break;
  case TO_ADD:
  case TO_SUB:
  case TO_MUL:
  case TO_DIV:
  case TO_MOD:
  case TO_NEG:
  case TO_BNOT:
  case TO_3CMP:
  case TO_AND:
  case TO_OR:
  case TO_XOR:
  case TO_SHL:
  case TO_SHR:
  case TO_USHR:
  case TO_INC:
    flags |= RT_NUMBER;
    break;
  case TO_CALL:
    flags |= RT_FUNCTION_CALL;
    break;
  case TO_DECL_EXPR:
    checkDeclaration(static_cast<const DeclExpr *>(arg));
    break;
  case TO_ARRAYEXPR:
    flags |= RT_ARRAY;
    break;
  default:
    flags |= RT_UNRECOGNIZED;
    break;
  }

  return true;
}

bool FunctionReturnTypeEvaluator::checkThrow(const ThrowStatement *thrw) {
  flags |= RT_THROW;
  return true;
}

bool FunctionReturnTypeEvaluator::checkIf(const IfStatement *ifStmt) {
  bool retThen = checkNode(ifStmt->thenBranch());
  bool retElse = false;
  if (ifStmt->elseBranch()) {
    retElse = checkNode(ifStmt->elseBranch());
  }

  return retThen && retElse;
}

bool FunctionReturnTypeEvaluator::checkLoop(const LoopStatement *loop) {
  checkNode(loop->body());
  return false;
}

bool FunctionReturnTypeEvaluator::checkBlock(const Block *block) {
  bool allReturns = false;
  
  for (const Statement *stmt : block->statements()) {
    allReturns |= checkNode(stmt);
  }

  return allReturns;
}

bool FunctionReturnTypeEvaluator::checkSwitch(const SwitchStatement *swtch) {
  bool allReturns = true;

  for (auto &c : swtch->cases()) {
    allReturns &= checkNode(c.stmt);
  }

  if (swtch->defaultCase().stmt) {
    allReturns &= checkNode(swtch->defaultCase().stmt);
  }

  return allReturns;
}

bool FunctionReturnTypeEvaluator::checkTry(const TryStatement *stmt) {
  bool retTry = checkNode(stmt->tryStatement());
  bool retCatch = checkNode(stmt->catchStatement());
  return retTry && retCatch;
}

class PredicateCheckerVisitor : public Visitor {
  bool deepCheck;
  bool result;
  const Node *checkee;
protected:
  NodeEqualChecker equalChecker;

  PredicateCheckerVisitor(bool deep) : deepCheck(deep) {}

  virtual bool doCheck(const Node *checkee, Node *n) const = 0;

public:
  void visitNode(Node *n) {
    if (doCheck(checkee, n)) {
      result = true;
      return;
    }

    if (deepCheck)
      n->visitChildren(this);
  }

  bool check(const Node *toCheck, Node *tree) {
    result = false;
    checkee = toCheck;
    tree->visit(this);
    return result;
  }
};

class CheckModificationVisitor : public PredicateCheckerVisitor {
protected:
  bool doCheck(const Node *checkee, Node *n) const {
    enum TreeOp op = n->op();

    if (op == TO_ASSIGN || op == TO_INEXPR_ASSIGN || (TO_PLUSEQ <= op && op <= TO_MODEQ)) {
      BinExpr *bin = static_cast<BinExpr *>(n);
      return equalChecker.check(checkee, bin->lhs());
    }

    if (op == TO_INC) {
      IncExpr *inc = static_cast<IncExpr *>(n);
      return equalChecker.check(checkee, inc->argument());
    }

    return false;
  }
public:
  CheckModificationVisitor() : PredicateCheckerVisitor(false) {}
};

class ExistsChecker : public PredicateCheckerVisitor {
protected:

  bool doCheck(const Node *checkee, Node *n) const {
    return equalChecker.check(checkee, n);
  }

public:

  ExistsChecker() : PredicateCheckerVisitor(true) {}
};

class ModificationChecker : public Visitor {
  bool result;
public:

  void visitNode(Node *n) {
    enum TreeOp op = n->op();

    switch (op)
    {
    case TO_INC:
    case TO_ASSIGN:
    case TO_INEXPR_ASSIGN:
    case TO_PLUSEQ:
    case TO_MINUSEQ:
    case TO_MULEQ:
    case TO_DIVEQ:
    case TO_MODEQ:
      result = true;
      return;
    default:
      n->visitChildren(this);
      break;
    }
  }

  bool check(Node *n) {
    result = false;
    n->visit(this);
    return result;
  }
};

static bool isBinaryArith(Expr *expr) {
  return TO_OROR <= expr->op() && expr->op() <= TO_SUB;
}

static bool isAssignExpr(Expr *expr) {
  enum TreeOp op = expr->op();
  return op == TO_ASSIGN || op == TO_INEXPR_ASSIGN || (TO_PLUSEQ <= op && op <= TO_MODEQ);
}

class ConditionalExitFinder : public Visitor {
  bool _firstLevel;
public:
  bool hasBreak;
  bool hasContinue;
  bool hasReturn;
  bool hasThrow;
  ConditionalExitFinder(bool firstLevel)
    : _firstLevel(firstLevel)
    , hasBreak(false)
    , hasContinue(false)
    , hasReturn(false)
    , hasThrow(false) {}

  void visitReturnStatement(ReturnStatement *stmt) {
    if (!hasReturn)
      hasReturn = !_firstLevel;
  }

  void visitThrowStatement(ThrowStatement *stmt) {
    if (!hasThrow)
      hasThrow = !_firstLevel;
  }

  void visitBreakStatement(BreakStatement *stmt) {
    if (!hasBreak)
      hasBreak = !_firstLevel;
  }

  void visitContinueStatement(ContinueStatement *stmt) {
    if (!hasContinue)
      hasContinue = !_firstLevel;
  }

  void visitIfStatement(IfStatement *stmt) {
    bool old = _firstLevel;
    _firstLevel = true;
    Visitor::visitIfStatement(stmt);
    _firstLevel = old;
  }
};

static bool isSuspiciousNeighborOfNullCoalescing(enum TreeOp op) {
  return (op == TO_3CMP || op == TO_ANDAND || op == TO_OROR || op == TO_IN || /*op == TO_NOTIN ||*/ op == TO_EQ || op == TO_NE || op == TO_LE ||
    op == TO_LT || op == TO_GT || op == TO_GE || op == TO_NOT || op == TO_BNOT || op == TO_AND || op == TO_OR ||
    op == TO_XOR || op == TO_DIV || op == TO_MOD || op == TO_INSTANCEOF || /*op == TO_QMARK ||*/ op == TO_NEG ||
    op == TO_ADD || op == TO_MUL || op == TO_SHL || op == TO_SHR || op == TO_USHR);
}

static bool isSuspiciousTernaryConditionOp(enum TreeOp op) {
  return op == TO_ADD || op == TO_SUB || op == TO_MUL || op == TO_DIV || op == TO_MOD ||
    op == TO_AND || op == TO_OR || op == TO_SHL || op == TO_SHR || op == TO_USHR || op == TO_3CMP;
}

static bool isSuspiciousSameOperandsBinaryOp(enum TreeOp op) {
  return op == TO_EQ || op == TO_LE || op == TO_LT || op == TO_GE || op == TO_GT || op == TO_NE ||
    op == TO_ANDAND || op == TO_OROR || op == TO_SUB || op == TO_3CMP || op == TO_DIV || op == TO_MOD ||
    op == TO_OR || op == TO_AND || op == TO_XOR || op == TO_SHL || op == TO_SHR || op == TO_USHR;
}

static bool isBlockTerminatorStatement(enum TreeOp op) {
  return op == TO_RETURN || op == TO_THROW || op == TO_BREAK || op == TO_CONTINUE;
}

static bool isBooleanResultOperator(enum TreeOp op) {
  return op == TO_OROR || op == TO_ANDAND || op == TO_NE || op == TO_EQ || (TO_GE <= op && op <= TO_IN) || op == TO_NOT;
}

static bool isArithOperator(enum TreeOp op) {
  return op == TO_OROR || op == TO_ANDAND
    || (TO_3CMP <= op && op <= TO_LT)
    || (TO_USHR <= op && op <= TO_SUB)
    || (TO_PLUSEQ <= op && op <= TO_MODEQ)
    || op == TO_BNOT || op == TO_NEG || op == TO_INC;
}

static bool isDivOperator(enum TreeOp op) {
  return op == TO_DIV || op == TO_MOD || op == TO_DIVEQ || op == TO_MODEQ;
}

bool isPureArithOperator(enum TreeOp op)
{
  return TO_USHR <= op && op <= TO_SUB || TO_PLUSEQ <= op && op <= TO_MODEQ;
}

bool isRelationOperator(enum TreeOp op)
{
  return TO_3CMP <= op && op <= TO_LT;
}

static const char *terminatorOpToName(enum TreeOp op) {
  switch (op)
  {
  case TO_BREAK: return "break";
  case TO_CONTINUE: return "continue";
  case TO_RETURN: return "return";
  case TO_THROW: return "throw";
  default:
    assert(0);
    return "<unkown terminator>";
  }
}

static const SQChar *function_can_return_string[] =
{
  "subst",
  "concat",
  "tostring",
  "toupper",
  "tolower",
  "slice",
  "trim",
  "join",
  "format",
  "replace",
  nullptr
};

static const SQChar *function_should_return_bool_prefix[] =
{
  "has",
  "Has",
  "have",
  "Have",
  "should",
  "Should",
  "need",
  "Need",
  "is",
  "Is",
  "was",
  "Was",
  "will",
  "Will",
  nullptr
};

static const SQChar *function_should_return_something_prefix[] =
{
  "get",
  "Get",
  nullptr
};


static bool hasPrefix(const SQChar *str, const SQChar *prefix, unsigned &l) {
  unsigned i = 0;

  for (;;) {
    SQChar c = str[i];
    SQChar p = prefix[i];

    if (!p) {
      l = i;
      return true;
    }

    if (!c) {
      return false;
    }

    if (c != p)
      return false;

    ++i;
  }
}

static bool hasAnyPrefix(const SQChar *str, const SQChar *prefixes[]) {
  for (int32_t i = 0; prefixes[i]; ++i) {
    unsigned l = 0;
    if (hasPrefix(str, prefixes[i], l)) {
      SQChar c = str[l];
      if (!c || c == '_' || c != tolower(c)) {
        return true;
      }
    }
  }

  return false;
}

static bool nameLooksLikeResultMustBeBoolean(const SQChar *funcName) {
  if (!funcName)
    return false;

  return hasAnyPrefix(funcName, function_should_return_bool_prefix);
}

static bool nameLooksLikeFunctionMustReturnResult(const SQChar *funcName) {
  if (!funcName)
    return false;

  bool nameInList = nameLooksLikeResultMustBeBoolean(funcName) ||
    hasAnyPrefix(funcName, function_should_return_something_prefix);

  if (!nameInList)
    if ((strstr(funcName, "_ctor") || strstr(funcName, "Ctor")) && strstr(funcName, "set") != funcName)
      nameInList = true;

  return nameInList;
}

static int32_t strhash(const SQChar *s) {
  int32_t r = 0;
  while (*s) {
    r *= 31;
    r += *s;
    ++s;
  }

  return r;
}

struct NameRef {
  const struct NameRef *receiver;
  const SQChar *name;

  NameRef(const NameRef *r, const SQChar *n) : receiver(r), name(n) {}

  bool equals(const NameRef *other) const {
    assert(other != nullptr);

    if (this == other)
      return true;

    if (receiver == other->receiver) {
      return strcmp(name, other->name) == 0;
    }

    if (!receiver || !other->receiver) {
      return false;
    }

    return receiver->equals(other->receiver) && strcmp(name, other->name) == 0;
  }

  bool operator==(const NameRef *other) const {
    return equals(other);
  }

  int32_t hashCode() const {
    int32_t p = receiver ? receiver->hashCode() : 0;
    return p * 31 + strhash(name);
  }

  struct Hasher {
    int32_t operator()(const NameRef *k) const {
      return k->hashCode();
    }
  };

  struct Comparator {
    bool operator()(const NameRef *l, const NameRef *r) const {
      return l->equals(r);
    }
  };
};

static NameRef rootRef = { nullptr, "::" };
static NameRef baseRef = { nullptr, "base" };
static NameRef thisRef = { nullptr, "this" };

enum ValueRefKind {
  VRK_UNDEFINED,
  VRK_EXPRESSION,
  //VRK_FUNCTION,
  //VRK_CLASS,
  //VRK_TABLE,
  //VRK_ARRAY,
  VRK_MULTIPLE,
  VRK_UNKNOWN,
  VRK_PARTIALLY
};

enum SymbolKind {
  SK_CONST,
  SK_BINDING,
  SK_VAR,
  SK_PARAM,
  SK_SLOT,
  SK_EXCEPTION // stands for catched exception
};

enum ValueBoundKind {
  VBK_UNKNOWN,
  VBK_INTEGER,
  VRK_FLOAT
};

struct ValueBound {
  enum ValueBoundKind kind;

  union {
    SQInteger i;
    SQFloat f;
  } v;
};

struct FunctionInfo {

  FunctionInfo(const NameRef *n, const FunctionDecl *d, const FunctionDecl *o) : nameRef(n), declaration(d), owner(o) {}

  struct Modifiable {
    const FunctionDecl *owner;
    const NameRef *name;
  };

  const FunctionDecl *owner;
  std::vector<Modifiable> modifible;
  const FunctionDecl *declaration;
  std::vector<const SQChar *> parameters;
  const NameRef *nameRef;

  void joinModifiable(const FunctionInfo *other);
  void addModifiable(const NameRef *name, const FunctionDecl *o);

};

void FunctionInfo::joinModifiable(const FunctionInfo *other) {
  for (auto &m : other->modifible) {
    if (owner == m.owner)
      continue;

    addModifiable(m.name, m.owner);
  }
}

void FunctionInfo::addModifiable(const NameRef *name, const FunctionDecl *o) {
  for (auto &m : modifible) {
    if (m.owner == o && m.name->equals(name))
      return;
  }

  modifible.push_back({ o, name });
}

struct ValueRef {
  enum ValueRefKind kind;
  const Expr *expression;

  const Decl *declaration;
  const FunctionDecl *owner;
  enum SymbolKind symKind;


  bool isConstant() const {
    return symKind != SK_VAR && symKind != SK_SLOT;
  }

  ValueBound lowerBound, upperBound;
  unsigned flagsPositive, flagsNegative;

  void kill(enum ValueRefKind k = VRK_UNKNOWN) {
    if (!isConstant()) {
      kind = k;
      expression = nullptr;
      flagsPositive = 0;
      flagsNegative = 0;
      lowerBound.kind = upperBound.kind = VBK_UNKNOWN;
    }
  }

  void merge(const ValueRef *other) {
    if (kind != other->kind) {
      enum ValueRefKind k = VRK_UNKNOWN;

      if (other->kind == VRK_EXPRESSION) {
        switch (kind)
        {
        case SQCompilation::VRK_UNDEFINED:
        case SQCompilation::VRK_PARTIALLY:
          k = VRK_PARTIALLY;
          break;
        case SQCompilation::VRK_MULTIPLE:
        case SQCompilation::VRK_UNKNOWN:
          k = VRK_MULTIPLE;
          break;
        default:
          break;
        }
      }

      kill(k);
      return;
    }

    if (isConstant()) {
      assert(other->isConstant());
      return;
    }

    NodeEqualChecker eqChecker;

    if (!NodeEqualChecker().check(expression, other->expression)) {
      kill(VRK_MULTIPLE);
    }
  }
};

struct VarScope {

  VarScope(const FunctionDecl *o, struct VarScope *p = nullptr) : owner(o), parent(p) {}

  const FunctionDecl *owner;
  struct VarScope *parent;
  std::unordered_map<const NameRef *, ValueRef *, NameRef::Hasher, NameRef::Comparator> symbols;

  void merge(const VarScope *other);
  VarScope *copy(Arena *a, bool forClosure = false) const;

  VarScope *findScope(const FunctionDecl *own);
};

void VarScope::merge(const VarScope *other) {
  VarScope *l = this;
  const VarScope *r = other;

  while (l) {
    assert(l->owner == r->owner && "Scope corruption");

    auto &thisSymbols = l->symbols;
    auto &otherSymbols = r->symbols;
    auto it = otherSymbols.begin();
    auto ie = otherSymbols.end();
    auto te = thisSymbols.end();

    while (it != ie) {
      auto f = thisSymbols.find(it->first);
      if (f != te) {
        f->second->merge(it->second);
      }
      else {
        it->second->kill(VRK_PARTIALLY);
        thisSymbols[it->first] = it->second;
      }
      ++it;
    }

    l = l->parent;
    r = r->parent;
  }
}

VarScope *VarScope::findScope(const FunctionDecl *own) {
  VarScope *s = this;

  while (s) {
    if (s->owner == own) {
      return s;
    }
    s = s->parent;
  }

  return nullptr;
}

VarScope *VarScope::copy(Arena *a, bool forClosure) const {
  VarScope *parentCopy = parent ? parent->copy(a, forClosure) : nullptr;
  void *mem = a->allocate(sizeof VarScope);
  VarScope *thisCopy = new(mem) VarScope(owner, parentCopy);

  for (auto &kv : symbols) {
    const NameRef *k = kv.first;
    ValueRef *v = kv.second;
    ValueRef *vcopy = nullptr;

    if (v->isConstant()) {
      vcopy = v;
    }
    else {
      void *mem = a->allocate(sizeof ValueRef);
      vcopy = new(mem) ValueRef();
      vcopy->symKind = v->symKind;
      if (forClosure) {
        // if we analyse closure we cannot rely on existed assignable values
        vcopy->kind = VRK_UNKNOWN;
        vcopy->expression = nullptr;
        vcopy->flagsNegative = vcopy->flagsPositive = 0;
        vcopy->lowerBound.kind = vcopy->upperBound.kind = VBK_UNKNOWN;
      }
      else {
        memcpy(vcopy, v, sizeof ValueRef);
      }
    }
    thisCopy->symbols[k] = vcopy;
  }

  return thisCopy;
}

class CheckerVisitor : public Visitor {
  SQCompilationContext &_ctx;

  NodeEqualChecker _equalChecker;

  bool isUpperCaseIdentifier(const Expr *expr);

  void report(const Node *n, enum DiagnosticsId id, ...);

  void checkAlwaysTrueOrFalse(const Node *expr);

  void checkAndOrPriority(const BinExpr *expr);
  void checkBitwiseBool(const BinExpr *expr);
  void checkCoalescingPriority(const BinExpr *expr);
  void checkAssignmentToItself(const BinExpr *expr);
  void checkGlobalVarCreation(const BinExpr *expr);
  void checkSameOperands(const BinExpr *expr);
  void checkAlwaysTrueOrFalse(const BinExpr *expr);
  void checkDeclarationInArith(const BinExpr *expr);
  void checkIntDivByZero(const BinExpr *expr);
  void checkPotentiallyNullableOperands(const BinExpr *expr);
  void checkAlwaysTrueOrFalse(const TerExpr *expr);
  void checkTernaryPriority(const TerExpr *expr);
  void checkSameValues(const TerExpr *expr);
  void checkExtendToAppend(const CallExpr *callExpr);
  void checkBoolIndex(const GetTableExpr *expr);

  bool findIfWithTheSameCondition(Expr * condition, IfStatement * elseNode) {
    if (_equalChecker.check(condition, elseNode->condition())) {
      return true;
    }

    Statement *elseB = elseNode->elseBranch();

    if (elseB && elseB->op() == TO_IF) {
      return findIfWithTheSameCondition(condition, (IfStatement *)elseB);
    }

    return false;
  }

  void checkDuplicateSwitchCases(SwitchStatement *swtch);
  void checkDuplicateIfBranches(IfStatement *ifStmt);
  void checkDuplicateIfConditions(IfStatement *ifStmt);

  bool onlyEmptyStatements(int32_t start, const ArenaVector<Statement *> &statements) {
    for (int32_t i = start; i < statements.size(); ++i) {
      Statement *stmt = statements[i];
      if (stmt->op() != TO_EMPTY)
        return false;
    }

    return true;
  }

  bool existsInTree(Expr *toSearch, Expr *tree) const {
    return ExistsChecker().check(toSearch, tree);
  }

  bool indexChangedInTree(Expr *index) const {
    return ModificationChecker().check(index);
  }

  bool checkModification(Expr *key, Node *tree) {
    return CheckModificationVisitor().check(key, tree);
  }

  void checkUnterminatedLoop(LoopStatement *loop);
  void checkVariableMismatchForLoop(ForStatement *loop);
  void checkEmptyWhileBody(WhileStatement *loop);
  void checkEmptyThenBody(IfStatement *stmt);
  void checkForgottenDo(const Block *block);
  void checkUnreachableCode(const Block *block);
  void checkAssigneTwice(const Block *b);
  void checkNullableContainer(const ForeachStatement *loop);

  const SQChar *findSlotNameInStack(const Decl *);
  void checkFunctionReturns(FunctionDecl *func);

  enum StackSlotType {
    SST_NODE,
    SST_TABLE_MEMBER
  };

  struct StackSlot {
    enum StackSlotType sst;
    union {
      const Node *n;
      const struct TableMember *member;
    };
  };

  std::vector<StackSlot> nodeStack;

  struct VarScope *currentScope;

  NameRef *makeNameRef(const NameRef *r, const SQChar *name) {
    void *mem = arena->allocate(sizeof NameRef);
    return new(mem) NameRef(r, name);
  }

  FunctionInfo *makeFunctionInfo(const NameRef *ref, const FunctionDecl *d, const FunctionDecl *o) {
    void *mem = arena->allocate(sizeof FunctionInfo);
    return new(mem) FunctionInfo(ref, d, o);
  }

  ValueRef *makeValueRef() {
    void *mem = arena->allocate(sizeof ValueRef);
    return new (mem) ValueRef();
  }

  std::unordered_map<const FunctionDecl *, FunctionInfo *> functionInfoMap;

  Arena *arena;

  FunctionInfo *currectInfo;

  void declareSymbol(const NameRef *name, const Expr *init, enum SymbolKind symkind, const FunctionDecl *owner, Decl *decl);
  void pushFunctionScope(VarScope *functionScope, const FunctionDecl *decl);

  void applyCallToScope(const CallExpr *call);
  void applyBinaryToScope(const BinExpr *bin);
  void applyAssignmentToScope(const BinExpr *bin);
  void applyNewslotToScope(const BinExpr *bin);
  void applyMemberToScope(const TableDecl *table, const TableMember &member);
  void applyAssignEqToScope(const BinExpr *bin);

  const NameRef *computeNameRef(const GetTableExpr *access);
  const NameRef *computeNameRef(const GetFieldExpr *access);
  const NameRef *computeNameRef(const Expr *lhs);

  ValueRef *findValueInScopes(const NameRef *ref);
  void applyKnownInvocationToScope(const ValueRef *ref);
  void applyUnknownInvocationToScope();

  bool isOwnedSymbol(const NameRef *name);

  void setExpression(const Expr *lvalue, const Expr *rvalue, unsigned pf = 0, unsigned nf = 0);
  const ValueRef *findValueForExpr(const Expr *e);
  const Expr *maybeEval(const Expr *e);

  bool isPotentiallyNullable(const Expr *e);

  void visitBinaryBranches(const BinExpr *expr, const Expr *v, unsigned pf, unsigned nf);

  LiteralExpr trueValue, falseValue, nullValue;

public:

  CheckerVisitor(SQCompilationContext &ctx)
    : _ctx(ctx)
    , arena(ctx.arena())
    , currectInfo(nullptr)
    , trueValue(true)
    , falseValue(false)
    , nullValue() {}

  void visitNode(Node *n);

  void visitBinExpr(BinExpr *expr);
  void visitTerExpr(TerExpr *expr);
  void visitCallExpr(CallExpr *expr);
  //void visitGetFieldExpr(GetFieldExpr *expr);
  void visitGetTableExpr(GetTableExpr *expr);

  void visitBlock(Block *b);
  void visitForStatement(ForStatement *loop);
  void visitForeachStatement(ForeachStatement *loop);
  void visitWhileStatement(WhileStatement *loop);
  void visitDoWhileStatement(DoWhileStatement *loop);
  void visitIfStatement(IfStatement *ifstmt);

  void visitSwitchStatement(SwitchStatement *swtch);

  void visitTryStatement(TryStatement *tryStmt);

  void visitFunctionDecl(FunctionDecl *func);

  void visitTableDecl(TableDecl *table);

  void visitVarDecl(VarDecl *decl);
  void visitConstDecl(ConstDecl *cnst);
  void visitEnumDecl(EnumDecl *enm);

  void analyse(RootBlock *root);
};

void CheckerVisitor::visitNode(Node *n) {
  nodeStack.push_back({ SST_NODE, n });
  Visitor::visitNode(n);
  nodeStack.pop_back();
}

void CheckerVisitor::report(const Node *n, enum DiagnosticsId id, ...) {
  va_list vargs;
  va_start(vargs, id);

  _ctx.vreportDiagnostic(id, n->lineStart(), n->columnStart(), n->columnEnd() - n->columnStart(), vargs);

  va_end(vargs);
}

bool CheckerVisitor::isUpperCaseIdentifier(const Expr *e) {

  const char *id = nullptr;

  if (e->op() == TO_GETFIELD) {
    id = e->asGetField()->fieldName();
  }
  else if (e->op() == TO_GETTABLE) {
    e = e->asGetTable()->key();
  }

  if (e->op() == TO_ID) {
    id = e->asId()->id();
  }
  else if (e->op() == TO_LITERAL && e->asLiteral()->kind() == LK_STRING) {
    id = e->asLiteral()->s();
  }

  if (!id)
    return false;

  while (*id) {
    if (*id >= 'a' && *id <= 'z')
      return false;
    ++id;
  }

  return true;
}

void CheckerVisitor::checkAndOrPriority(const BinExpr *expr) {
  const Expr *l = expr->lhs();
  const Expr *r = expr->rhs();

  if (expr->op() == TO_OROR) {
    if (l->op() == TO_ANDAND || r->op() == TO_ANDAND) {
      report(expr, DiagnosticsId::DI_AND_OR_PAREN);
    }
  }
}

void CheckerVisitor::checkBitwiseBool(const BinExpr *expr) {
  const Expr *l = expr->lhs();
  const Expr *r = expr->rhs();

  if (expr->op() == TO_OROR || expr->op() == TO_ANDAND) {
    if (l->op() == TO_AND || l->op() == TO_OR || r->op() == TO_AND || r->op() == TO_OR) {
      report(expr, DiagnosticsId::DI_BITWISE_BOOL_PAREN);
    }
  }
}

void CheckerVisitor::checkCoalescingPriority(const BinExpr *expr) {
  const Expr *l = expr->lhs();
  const Expr *r = expr->rhs();

  if (expr->op() == TO_NULLC) {
    if (isSuspiciousNeighborOfNullCoalescing(l->op())) {
      report(l, DiagnosticsId::DI_NULL_COALSESSING_PRIOR, treeopStr(l->op()));
    }

    if (isSuspiciousNeighborOfNullCoalescing(r->op())) {
      report(r, DiagnosticsId::DI_NULL_COALSESSING_PRIOR, treeopStr(r->op()));
    }
  }
}

void CheckerVisitor::checkAssignmentToItself(const BinExpr *expr) {
  const Expr *l = expr->lhs();
  const Expr *r = expr->rhs();

  if (expr->op() == TO_ASSIGN) {
    auto *ll = deparen(l);
    auto *rr = deparen(r);

    if (_equalChecker.check(ll, rr)) {
      report(expr, DiagnosticsId::DI_ASG_TO_ITSELF);
    }
  }
}

void CheckerVisitor::checkGlobalVarCreation(const BinExpr *expr) {
  const Expr *l = expr->lhs();

  if (expr->op() == TO_NEWSLOT) {
    if (l->op() == TO_ID) {
      report(expr, DiagnosticsId::DI_GLOBAL_VAR_CREATE);
    }
  }
}

void CheckerVisitor::checkSameOperands(const BinExpr *expr) {
  const Expr *l = expr->lhs();
  const Expr *r = expr->rhs();

  if (isSuspiciousSameOperandsBinaryOp(expr->op())) {
    const Expr *ll = deparen(l);
    const Expr *rr = deparen(r);

    if (_equalChecker.check(ll, rr)) {
      if (ll->op() != TO_LITERAL || (ll->asLiteral()->kind() != LK_FLOAT && ll->asLiteral()->kind() != LK_INT)) {
        report(expr, DiagnosticsId::DI_SAME_OPERANDS, treeopStr(expr->op()));
      }
    }
  }
}

void CheckerVisitor::checkAlwaysTrueOrFalse(const Node *n) {
  const Node *cond = n->isExpression() ? maybeEval(n->asExpression()) : n;

  if (cond->op() == TO_LITERAL) {
    const LiteralExpr *l = cond->asExpression()->asLiteral();
    report(n, DiagnosticsId::DI_ALWAYS_T_OR_F, l->raw() ? "true" : "false");
  }
  else if (cond->op() == TO_ARRAYEXPR || cond->op() == TO_DECL_EXPR || cond->isDeclaration()) {
    report(n, DiagnosticsId::DI_ALWAYS_T_OR_F, "true");
  }
}

void CheckerVisitor::checkAlwaysTrueOrFalse(const TerExpr *expr) {
  checkAlwaysTrueOrFalse(expr->a());
}

void CheckerVisitor::checkTernaryPriority(const TerExpr *expr) {
  const Expr *cond = expr->a();
  const Expr *thenExpr = expr->b();
  const Expr *elseExpr = expr->c();

  if (isSuspiciousTernaryConditionOp(cond->op())) {
    const BinExpr *binCond = static_cast<const BinExpr *>(cond);
    const Expr *l = binCond->lhs();
    const Expr *r = binCond->rhs();
    bool isIgnore = cond->op() == TO_AND
      && (isUpperCaseIdentifier(l) || isUpperCaseIdentifier(r)
        || (r->op() == TO_LITERAL && r->asLiteral()->kind() == LK_INT));

    if (!isIgnore) {
      report(cond, DiagnosticsId::DI_TERNARY_PRIOR, treeopStr(cond->op()));
    }
  }
}

void CheckerVisitor::checkSameValues(const TerExpr *expr) {
  const Expr *ifTrue = maybeEval(expr->b())->asExpression();
  const Expr *ifFalse = maybeEval(expr->c())->asExpression();

  if (_equalChecker.check(ifTrue, ifFalse)) {
    report(expr, DiagnosticsId::DI_TERNARY_SAME_VALUES);
  }
}

void CheckerVisitor::checkAlwaysTrueOrFalse(const BinExpr *bin) {
  enum TreeOp op = bin->op();
  if (op != TO_ANDAND && op != TO_OROR)
    return;

  const Expr *lhs = maybeEval(bin->lhs())->asExpression();
  const Expr *rhs = maybeEval(bin->rhs())->asExpression();

  enum TreeOp cmpOp = lhs->op();
  if (cmpOp == rhs->op() && (cmpOp == TO_NE || cmpOp == TO_EQ)) {
    const char *constValue = nullptr;

    if (op == TO_ANDAND && cmpOp == TO_EQ)
      constValue = "false";

    if (op == TO_OROR && cmpOp == TO_NE)
      constValue = "true";

    if (!constValue)
      return;

    const BinExpr *lhsBin = static_cast<const BinExpr *>(lhs);
    const BinExpr *rhsBin = static_cast<const BinExpr *>(rhs);

    const Expr *lconst = lhsBin->rhs();
    const Expr *rconst = rhsBin->rhs();

    if (lconst->op() == TO_LITERAL || isUpperCaseIdentifier(lconst)) {
      if (rconst->op() == TO_LITERAL || isUpperCaseIdentifier(rconst)) {
        if (_equalChecker.check(lhsBin->lhs(), rhsBin->lhs())) {
          if (!_equalChecker.check(lconst, rconst)) {
            report(bin, DiagnosticsId::DI_ALWAYS_T_OR_F, constValue);
          }
        }
      }
    }
  }
  else if (lhs->op() == TO_NOT || rhs->op() == TO_NOT && (lhs->op() != rhs->op())) {
    const char *v = op == TO_OROR ? "true" : "false";
    if (lhs->op() == TO_NOT) {
      const UnExpr *u = static_cast<const UnExpr *>(lhs);
      if (_equalChecker.check(u->argument(), rhs)) {
        report(bin, DiagnosticsId::DI_ALWAYS_T_OR_F, v);
      }
    }
    if (rhs->op() == TO_NOT) {
      const UnExpr *u = static_cast<const UnExpr *>(rhs);
      if (_equalChecker.check(lhs, u->argument())) {
        report(bin, DiagnosticsId::DI_ALWAYS_T_OR_F, v);
      }
    }
  }
}

void CheckerVisitor::checkDeclarationInArith(const BinExpr *bin) {
  if (isArithOperator(bin->op())) {
    const Expr *lhs = maybeEval(bin->lhs());
    const Expr *rhs = maybeEval(bin->rhs());

    if (lhs->op() == TO_DECL_EXPR || lhs->op() == TO_ARRAYEXPR) {
      report(bin->lhs(), DiagnosticsId::DI_DECL_IN_EXPR);
    }

    if (rhs->op() == TO_DECL_EXPR || rhs->op() == TO_ARRAYEXPR) {
      report(bin->rhs(), DiagnosticsId::DI_DECL_IN_EXPR);
    }
  }
}

void CheckerVisitor::checkIntDivByZero(const BinExpr *bin) {
  if (isDivOperator(bin->op())) {
    const Expr *divisor = maybeEval(bin->rhs());
    if (divisor->op() == TO_LITERAL) {
      const LiteralExpr *l = divisor->asLiteral();
      if (l->kind() == LK_INT || l->kind() == LK_BOOL) {
        if (l->raw() == 0) {
          report(bin, DiagnosticsId::DI_DIV_BY_ZERO);
        }
      }
    }
  }
}

void CheckerVisitor::checkPotentiallyNullableOperands(const BinExpr *bin) {

  bool isRelOp = isRelationOperator(bin->op());
  bool isArithOp = isPureArithOperator(bin->op());

  if (!isRelOp && !isArithOp)
    return;

  const Expr *lhs = skipUnary(maybeEval(skipUnary(bin->lhs())));
  const Expr *rhs = skipUnary(maybeEval(skipUnary(bin->rhs())));

  const char *opType = isRelOp ? "Comparison operation" : "Arithmetic operation";

  if (isPotentiallyNullable(lhs)) {
    report(bin->lhs(), DiagnosticsId::DI_NULLABLE_OPERANDS, opType);
  }

  if (isPotentiallyNullable(rhs)) {
    report(bin->rhs(), DiagnosticsId::DI_NULLABLE_OPERANDS, opType);
  }
}

void CheckerVisitor::checkExtendToAppend(const CallExpr *expr) {
  const Expr *callee = expr->callee();
  const auto &args = expr->arguments();

  if (callee->op() == TO_GETFIELD) {
    if (args.size() > 0) {
      Expr *arg0 = args[0];
      if (arg0->op() == TO_ARRAYEXPR) {
        if (strcmp(callee->asGetField()->fieldName(), "extend") == 0) {
          report(expr, DiagnosticsId::DI_EXTEND_TO_APPEND);
        }
      }
    }
  }
}

void CheckerVisitor::visitBinaryBranches(const BinExpr *expr, const Expr *v, unsigned pf, unsigned nf) {
  Expr *lhs = expr->lhs();
  Expr *rhs = expr->rhs();
  VarScope *trunkScope = currentScope;
  VarScope *branchScope = nullptr;

  lhs->visit(this);
  currentScope = branchScope = trunkScope->copy(arena);
  setExpression(lhs, v, pf, nf);
  rhs->visit(this);
  trunkScope->merge(branchScope);
  currentScope = trunkScope;
}

void CheckerVisitor::visitBinExpr(BinExpr *expr) {

  applyBinaryToScope(expr);

  checkAndOrPriority(expr);
  checkBitwiseBool(expr);
  checkCoalescingPriority(expr);
  checkAssignmentToItself(expr);
  checkGlobalVarCreation(expr);
  checkSameOperands(expr);
  checkAlwaysTrueOrFalse(expr);
  checkDeclarationInArith(expr);
  checkIntDivByZero(expr);
  checkPotentiallyNullableOperands(expr);

  switch (expr->op())
  {
  case TO_NULLC:
    visitBinaryBranches(expr, &nullValue, RT_NULL, 0);
    break;
  case TO_ANDAND:
    visitBinaryBranches(expr, &trueValue, 0, RT_NULL);
    break;
  case TO_OROR:
    visitBinaryBranches(expr, &falseValue, RT_NULL, 0);
    break;
  default:
    Visitor::visitBinExpr(expr);
    break;
  }
}

void CheckerVisitor::visitTerExpr(TerExpr *expr) {
  checkTernaryPriority(expr);
  checkSameValues(expr);
  checkAlwaysTrueOrFalse(expr);

  expr->a()->visit(this);
  VarScope *trunkScope = currentScope;

  VarScope *ifTrueScope = trunkScope->copy(arena);
  currentScope = ifTrueScope;
  setExpression(expr->a(), &trueValue, 0, RT_NULL);
  expr->b()->visit(this);

  VarScope *ifFalseScope = trunkScope->copy(arena);
  currentScope = ifFalseScope;
  setExpression(expr->a(), &falseValue, RT_NULL, 0);
  expr->c()->visit(this);

  trunkScope->merge(ifTrueScope);
  trunkScope->merge(ifFalseScope);
  currentScope = trunkScope;
}

void CheckerVisitor::visitCallExpr(CallExpr *expr) {

  applyCallToScope(expr);

  checkExtendToAppend(expr);

  Visitor::visitCallExpr(expr);
}

void CheckerVisitor::checkBoolIndex(const GetTableExpr *expr) {
  const Expr *key = maybeEval(expr->key())->asExpression();

  if (isBooleanResultOperator(key->op())) {
    report(expr->key(), DiagnosticsId::DI_BOOL_AS_INDEX);
  }
}

void CheckerVisitor::visitGetTableExpr(GetTableExpr *expr) {
  checkBoolIndex(expr);

  Visitor::visitGetTableExpr(expr);
}

void CheckerVisitor::checkDuplicateSwitchCases(SwitchStatement *swtch) {
  const auto &cases = swtch->cases();

  for (int32_t i = 0; i < cases.size() - 1; ++i) {
    for (int32_t j = i + 1; j < cases.size(); ++j) {
      const auto &icase = cases[i];
      const auto &jcase = cases[j];

      if (_equalChecker.check(icase.val, jcase.val)) {
        report(jcase.val, DiagnosticsId::DI_DUPLICATE_CASE);
      }
    }
  }
}

void CheckerVisitor::checkDuplicateIfBranches(IfStatement *ifStmt) {
  if (_equalChecker.check(ifStmt->thenBranch(), ifStmt->elseBranch())) {
    report(ifStmt->elseBranch(), DiagnosticsId::DI_THEN_ELSE_EQUAL);
  }
}

void CheckerVisitor::checkDuplicateIfConditions(IfStatement *ifStmt) {
  Statement *elseB = ifStmt->elseBranch();

  if (elseB && elseB->op() == TO_IF) {
    if (findIfWithTheSameCondition(ifStmt->condition(), (IfStatement *)elseB)) {
      report(ifStmt->condition(), DiagnosticsId::DI_DUPLICATE_IF_EXPR);
    }
  }
}

void CheckerVisitor::checkVariableMismatchForLoop(ForStatement *loop) {
  const SQChar *varname = nullptr;
  Node *init = loop->initializer();
  Expr *cond = loop->condition();
  Expr *mod = loop->modifier();

  if (init) {
    if (init->op() == TO_ASSIGN) {
      Expr *l = static_cast<BinExpr *>(init)->lhs();
      if (l->op() == TO_ID)
        varname = l->asId()->id();
    }

    if (init->op() == TO_VAR) {
      VarDecl *decl = static_cast<VarDecl *>(init);
      varname = decl->name();
    }
  }

  if (varname && cond) {
    if (isBinaryArith(cond)) {
      BinExpr *bin = static_cast<BinExpr *>(cond);
      Expr *l = bin->lhs();
      Expr *r = bin->rhs();
      bool idUsed = false;

      if (l->op() == TO_ID) {
        if (strcmp(l->asId()->id(), varname)) {
          if (r->op() != TO_ID || strcmp(r->asId()->id(), varname)) {
            report(cond, DiagnosticsId::DI_MISMATCH_LOOP_VAR);
          }
        }
      }
    }
  }

  if (varname && mod) {
    bool idUsed = false;
    if (isAssignExpr(mod)) {
      Expr *lhs = static_cast<BinExpr *>(mod)->lhs();
      if (lhs->op() == TO_ID) {
        if (strcmp(varname, lhs->asId()->id())) {
          report(mod, DiagnosticsId::DI_MISMATCH_LOOP_VAR);
        }
      }
    }

    if (!idUsed && mod->op() == TO_INC) {
      Expr *arg = static_cast<IncExpr *>(mod)->argument();
      if (arg->op() == TO_ID) {
        if (strcmp(varname, arg->asId()->id())) {
          report(mod, DiagnosticsId::DI_MISMATCH_LOOP_VAR);
        }
      }
    }
  }
}

void CheckerVisitor::checkUnterminatedLoop(LoopStatement *loop) {
  Statement *body = loop->body();

  ReturnStatement *retStmt = nullptr;
  ThrowStatement *throwStmt = nullptr;
  BreakStatement *breakStmt = nullptr;
  ContinueStatement *continueStmt = nullptr;

  switch (body->op())
  {
  case TO_BLOCK: {
    Block *b = static_cast<Block *>(body);
    for (Statement *stmt : b->statements()) {
      switch (stmt->op())
      {
      case TO_RETURN: retStmt = static_cast<ReturnStatement *>(stmt); break;
      case TO_THROW: throwStmt = static_cast<ThrowStatement *>(stmt); break;
      case TO_BREAK: breakStmt = static_cast<BreakStatement *>(stmt); break;
      case TO_CONTINUE: continueStmt = static_cast<ContinueStatement *>(stmt); break;
      default:
        break;
      }
    }
    break;
  }
  case TO_RETURN: retStmt = static_cast<ReturnStatement *>(body); break;
  case TO_THROW: throwStmt = static_cast<ThrowStatement *>(body); break;
  case TO_BREAK: breakStmt = static_cast<BreakStatement *>(body); break;
  case TO_CONTINUE: continueStmt = static_cast<ContinueStatement *>(body); break;
  default:
    loop->visitChildren(this);
    return;
  }

  if (retStmt || throwStmt || breakStmt || continueStmt) {
    ConditionalExitFinder checker(false);
    body->visit(&checker);

    if (retStmt) {
      if (!checker.hasBreak && !checker.hasContinue && !checker.hasThrow && loop->op() != TO_FOREACH) {
        report(retStmt, DiagnosticsId::DI_UNCOND_TERMINATED_LOOP, "return");
      }
    }

    if (throwStmt) {
      if (!checker.hasBreak && !checker.hasContinue && !checker.hasReturn && loop->op() != TO_FOREACH) {
        report(throwStmt, DiagnosticsId::DI_UNCOND_TERMINATED_LOOP, "throw");
      }
    }

    if (continueStmt) {
      if (!checker.hasBreak && !checker.hasThrow && !checker.hasReturn) {
        report(continueStmt, DiagnosticsId::DI_UNCOND_TERMINATED_LOOP, "continue");
      }
    }

    if (breakStmt) {
      if (!checker.hasContinue && !checker.hasReturn && !checker.hasThrow && loop->op() != TO_FOREACH) {
        report(breakStmt, DiagnosticsId::DI_UNCOND_TERMINATED_LOOP, "break");
      }
    }
  }
}

void CheckerVisitor::checkEmptyWhileBody(WhileStatement *loop) {
  const Statement *body = unwrapBody(loop->body());

  if (body && body->op() == TO_EMPTY) {
    report(body, DiagnosticsId::DI_EMPTY_BODY, "while");
  }
}

void CheckerVisitor::checkEmptyThenBody(IfStatement *stmt) {
  const Statement *thenStmt = unwrapBody(stmt->thenBranch());

  if (thenStmt && thenStmt->op() == TO_EMPTY) {
    report(thenStmt, DiagnosticsId::DI_EMPTY_BODY, "then");
  }
}

void CheckerVisitor::checkAssigneTwice(const Block *b) {

  const auto &statements = b->statements();

  for (int32_t i = 0; i < int32_t(statements.size()) - 1; ++i) {
    Expr *iexpr = unwrapExprStatement(statements[i]);

    if (iexpr && iexpr->op() == TO_ASSIGN) {
      for (int32_t j = i + 1; j < statements.size(); ++j) {
        Expr *jexpr = unwrapExprStatement(statements[j]);

        if (jexpr && jexpr->op() == TO_ASSIGN) {
          BinExpr *iassgn = static_cast<BinExpr *>(iexpr);
          BinExpr *jassgn = static_cast<BinExpr *>(jexpr);

          if (_equalChecker.check(iassgn->lhs(), jassgn->lhs())) {
            Expr *firstAssignee = iassgn->lhs();

            bool ignore = existsInTree(firstAssignee, jassgn->rhs());
            if (!ignore && firstAssignee->op() == TO_GETTABLE) {
              GetTableExpr *getT = firstAssignee->asGetTable();
              ignore = indexChangedInTree(getT->key());
              if (!ignore) {
                for (int32_t m = i + 1; m < j; ++m) {
                  if (checkModification(getT->key(), statements[m])) {
                    ignore = true;
                    break;
                  }
                }
              }
            }

            if (!ignore) {
              report(jassgn, DiagnosticsId::DI_ASSIGNED_TWICE);
            }
          }
        }
      }
    }
  }
}

void CheckerVisitor::checkForgottenDo(const Block *b) {
  const auto &statements = b->statements();

  for (int32_t i = 1; i < statements.size(); ++i) {
    Statement *prev = statements[i - 1];
    Statement *stmt = statements[i];

    if (stmt->op() == TO_WHILE && prev->op() == TO_BLOCK) {
      report(stmt, DiagnosticsId::DI_FORGOTTEN_DO);
    }
  }
}

void CheckerVisitor::checkUnreachableCode(const Block *block) {
  const auto &statements = block->statements();

  for (int32_t i = 0; i < int32_t(statements.size()) - 1; ++i) {
    Statement *stmt = statements[i];
    Statement *next = statements[i + 1];
    if (isBlockTerminatorStatement(stmt->op())) {
      if (next->op() != TO_BREAK) {
        if (!onlyEmptyStatements(i + 1, statements)) {
          report(next, DiagnosticsId::DI_UNREACHABLE_CODE, terminatorOpToName(stmt->op()));
          break;
        }
      }
    }
  }
}

void CheckerVisitor::visitBlock(Block *b) {
  checkForgottenDo(b);
  checkUnreachableCode(b);
  checkAssigneTwice(b);

  Visitor::visitBlock(b);
}

void CheckerVisitor::visitForStatement(ForStatement *loop) {
  checkUnterminatedLoop(loop);
  checkVariableMismatchForLoop(loop);

  VarScope *trunkScope = currentScope;

  Node *init = loop->initializer();
  Expr *cond = loop->condition();
  Expr *mod = loop->modifier();

  if (init && !init->isDeclaration()) {
    init->visit(this);
  }

  VarScope *loopScope = trunkScope->copy(arena);
  currentScope = loopScope;

  if (init && init->isDeclaration()) {
    init->visit(this);
  }

  if (cond) {
    cond->visit(this);
    setExpression(cond, &trueValue, 0, RT_NULL);
  }

  loop->body()->visit(this);

  if (mod) {
    mod->visit(this);
  }

  trunkScope->merge(loopScope);
  currentScope = trunkScope;
  if (cond)
    setExpression(cond, &falseValue, RT_NULL, 0);
}

void CheckerVisitor::checkNullableContainer(const ForeachStatement *loop) {
  if (isPotentiallyNullable(loop->container())) {
    report(loop->container(), DiagnosticsId::DI_POTENTIALLY_NULLABLE_CONTAINER);
  }
}

void CheckerVisitor::visitForeachStatement(ForeachStatement *loop) {
  checkUnterminatedLoop(loop);
  checkNullableContainer(loop);

  VarScope *trunkScope = currentScope;
  VarScope *loopScope = trunkScope->copy(arena);
  currentScope = loopScope;

  Visitor::visitForeachStatement(loop);

  trunkScope->merge(loopScope);
  currentScope = trunkScope;
}

void CheckerVisitor::visitWhileStatement(WhileStatement *loop) {
  checkUnterminatedLoop(loop);
  checkEmptyWhileBody(loop);

  loop->condition()->visit(this);

  VarScope *trunkScope = currentScope;
  VarScope *loopScope = trunkScope->copy(arena);
  currentScope = loopScope;
  // TODO: set condition -- true

  setExpression(loop->condition(), &trueValue, 0, RT_NULL);
  loop->body()->visit(this);

  trunkScope->merge(loopScope);
  currentScope = trunkScope;
  setExpression(loop->condition(), &falseValue, RT_NULL, 0);
}

void CheckerVisitor::visitDoWhileStatement(DoWhileStatement *loop) {
  checkUnterminatedLoop(loop);

  VarScope *trunkScope = currentScope;
  VarScope *loopScope = trunkScope->copy(arena);
  currentScope = loopScope;

  loop->body()->visit(this);
  loop->condition()->visit(this);

  trunkScope->merge(loopScope);
  currentScope = trunkScope;
  setExpression(loop->condition(), &falseValue, RT_NULL, 0);
}

void CheckerVisitor::visitIfStatement(IfStatement *ifstmt) {
  checkEmptyThenBody(ifstmt);
  checkDuplicateIfConditions(ifstmt);
  checkDuplicateIfBranches(ifstmt);
  checkAlwaysTrueOrFalse(ifstmt->condition());

  ifstmt->condition()->visit(this);

  VarScope *trunkScope = currentScope;
  VarScope *thenScope = trunkScope->copy(arena);
  currentScope = thenScope;
  setExpression(ifstmt->condition(), &trueValue, 0, RT_NULL);
  ifstmt->thenBranch()->visit(this);
  VarScope *elseScope = nullptr;
  if (ifstmt->elseBranch()) {
    currentScope = elseScope = trunkScope->copy(arena);
    setExpression(ifstmt->condition(), &falseValue, RT_NULL, 0);
    ifstmt->elseBranch()->visit(this);
  }

  trunkScope->merge(thenScope);
  if (elseScope)
    trunkScope->merge(elseScope);

  currentScope = trunkScope;
}

void CheckerVisitor::visitSwitchStatement(SwitchStatement *swtch) {
  checkDuplicateSwitchCases(swtch);

  Expr *expr = swtch->expression();
  expr->visit(this);

  auto &cases = swtch->cases();
  VarScope *trunkScope = currentScope;

  std::vector<VarScope *> scopes;

  for (auto &c : cases) {
    c.val->visit(this);
    VarScope *caseScope = trunkScope->copy(arena);
    currentScope = caseScope;
    setExpression(expr, c.val);
    c.stmt->visit(this);
    scopes.push_back(caseScope);
  }

  if (swtch->defaultCase().stmt) {
    VarScope *defaultScope = trunkScope->copy(arena);
    currentScope = defaultScope;
    swtch->defaultCase().stmt->visit(this);
    scopes.push_back(defaultScope);
  }

  for (VarScope *s : scopes) {
    trunkScope->merge(s);
  }

  currentScope = trunkScope;
}

void CheckerVisitor::visitTryStatement(TryStatement *tryStmt) {

  Statement *t = tryStmt->tryStatement();
  Id *id = tryStmt->exceptionId();
  Statement *c = tryStmt->catchStatement();

  VarScope *trunkScope = currentScope;
  VarScope *tryScope = trunkScope->copy(arena);
  currentScope = tryScope;
  t->visit(this);

  VarScope *catchScope = trunkScope->copy(arena);
  currentScope = catchScope;
  declareSymbol(makeNameRef(nullptr, id->id()), nullptr, SK_EXCEPTION, trunkScope->owner, nullptr);
  id->visit(this);
  c->visit(this);

  trunkScope->merge(tryScope);
  trunkScope->merge(catchScope);
  currentScope = trunkScope;
}

const SQChar *CheckerVisitor::findSlotNameInStack(const Decl *decl) {
  auto it = nodeStack.rbegin();
  auto ie = nodeStack.rend();

  while (it != ie) {
    auto slot = *it;
    if (slot.sst == SST_NODE) {
      const Node *n = slot.n;
      if (n->op() == TO_NEWSLOT) {
        const BinExpr *bin = static_cast<const BinExpr *>(n);
        Expr *lhs = bin->lhs();
        Expr *rhs = bin->rhs();
        if (rhs->op() == TO_DECL_EXPR) {
          const DeclExpr *de = static_cast<const DeclExpr *>(rhs);
          if (de->declaration() == decl) {
            if (lhs->op() == TO_LITERAL) {
              if (lhs->asLiteral()->kind() == LK_STRING) {
                return lhs->asLiteral()->s();
              }
            }
          }
          return nullptr;
        }
      }
    }
    else {
      assert(slot.sst == SST_TABLE_MEMBER);
      Expr *lhs = slot.member->key;
      Expr *rhs = slot.member->value;
      if (rhs->op() == TO_DECL_EXPR) {
        const DeclExpr *de = static_cast<const DeclExpr *>(rhs);
        if (de->declaration() == decl) {
          if (lhs->op() == TO_LITERAL) {
            if (lhs->asLiteral()->kind() == LK_STRING) {
              return lhs->asLiteral()->s();
            }
          }
        }
        return nullptr;
      }
    }
    ++it;
  }

  return nullptr;
}

void CheckerVisitor::checkFunctionReturns(FunctionDecl *func) {
  const SQChar *name = func->name();

  if (!name || name[0] == '(') {
    name = findSlotNameInStack(func);
  }

  unsigned returnFlags = FunctionReturnTypeEvaluator().compute(func->body());

  bool reported = false;

  if (returnFlags & ~(RT_BOOL | RT_UNRECOGNIZED | RT_FUNCTION_CALL)) {
    if (nameLooksLikeResultMustBeBoolean(name)) {
      report(func, DiagnosticsId::DI_NAME_RET_BOOL, name);
      reported = true;
    }
  }

  if (!!(returnFlags & RT_NOTHING) &&
    !!(returnFlags & (RT_NUMBER | RT_STRING | RT_TABLE | RT_CLASS | RT_ARRAY | RT_CLOSURE | RT_UNRECOGNIZED | RT_THROW)))
  {
    if ((returnFlags & RT_THROW) == 0)
      report(func, DiagnosticsId::DI_NOT_ALL_PATH_RETURN_VALUE);
    reported = true;
  }
  else if (returnFlags)
  {
    unsigned flagsDiff = returnFlags & ~(RT_THROW | RT_NOTHING | RT_NULL | RT_UNRECOGNIZED | RT_FUNCTION_CALL);
    if (flagsDiff)
    {
      bool powerOfTwo = !(flagsDiff == 0) && !(flagsDiff & (flagsDiff - 1));
      if (!powerOfTwo)
      {
        report(func, DiagnosticsId::DI_RETURNS_DIFFERENT_TYPES);
        reported = true;
      }
    }
  }

  if (!reported) {
    if (!!(returnFlags & RT_NOTHING) && nameLooksLikeFunctionMustReturnResult(name)) {
      report(func, DiagnosticsId::DI_NAME_EXPECTS_RETURN, name);
    }
  }
}

void CheckerVisitor::visitTableDecl(TableDecl *table) {
  for (auto &member : table->members()) {
    StackSlot slot;
    slot.sst = SST_TABLE_MEMBER;
    slot.member = &member;
    applyMemberToScope(table, member);
    nodeStack.push_back(slot);
    member.key->visit(this);
    member.value->visit(this);
    nodeStack.pop_back();
  }
}

void CheckerVisitor::visitFunctionDecl(FunctionDecl *func) {
  VarScope *parentScope = currentScope;
  VarScope *copyScope = parentScope->copy(arena, true);
  VarScope functionScope(func, copyScope);

  pushFunctionScope(&functionScope, func);

  FunctionInfo *oldInfo = currectInfo;
  FunctionInfo *newInfo = functionInfoMap[func];

  currectInfo = newInfo;
  assert(newInfo);

  checkFunctionReturns(func);

  Visitor::visitFunctionDecl(func);

  if (oldInfo) {
    oldInfo->joinModifiable(newInfo);
  }

  currectInfo = oldInfo;
  currentScope = parentScope;
}

ValueRef *CheckerVisitor::findValueInScopes(const NameRef *ref) {
  if (!ref)
    return nullptr;

  VarScope *current = currentScope;
  VarScope *s = current;

  while (s) {
    auto &symbols = s->symbols;
    auto it = symbols.find(ref);
    if (it != symbols.end()) {
      return it->second;
    }

    s = s->parent;
  }

  return nullptr;
}

void CheckerVisitor::applyMemberToScope(const TableDecl *table, const TableMember &member) {
  const Expr *lhs = member.key;
  const Expr *rhs = member.value;

  if (lhs->op() != TO_LITERAL)
    return;

  const LiteralExpr *l = lhs->asLiteral();

  if (l->kind() == LK_STRING)
    return;

  const SQChar *field = l->s();
  const SQChar *tableName = findSlotNameInStack(table);

  if (!tableName)
    return;

  const NameRef *nameRef = makeNameRef(makeNameRef(nullptr, tableName), field);

  ValueRef *v = makeValueRef();
  v->symKind = SK_SLOT;
  v->expression = rhs;
  v->declaration = table;
  v->owner = currentScope->owner;
  v->kind = VRK_EXPRESSION;

  currentScope->symbols[nameRef] = v;
}

void CheckerVisitor::applyAssignmentToScope(const BinExpr *bin) {
  assert(bin->op() == TO_ASSIGN || bin->op() == TO_INEXPR_ASSIGN || bin->op() == TO_NEWSLOT);

  const Expr *lhs = bin->lhs();
  const Expr *rhs = bin->rhs();

  const NameRef *name = computeNameRef(lhs);
  if (!name)
    return;

  ValueRef *v = findValueInScopes(name);

  if (!v) {
    v = makeValueRef();
    currentScope->symbols[name] = v;
    if (currectInfo) {
      currectInfo->addModifiable(name, v->owner);
    }
    v->symKind = bin->op() == TO_NEWSLOT ? SK_SLOT : SK_VAR;
    v->owner = currentScope->owner;
  }

  v->expression = rhs;
  v->kind = VRK_EXPRESSION;
  v->flagsNegative = v->flagsPositive = 0;
  v->lowerBound.kind = v->upperBound.kind = VBK_UNKNOWN;

  //computeValueRef(name, v, rhs, currentScope->owner);
}

void CheckerVisitor::applyNewslotToScope(const BinExpr *bin) {
  assert(bin->op() == TO_NEWSLOT);

  const Expr *table = bin->lhs();
  const Expr *value = bin->rhs();

  const NameRef *nameRef = computeNameRef(table);


  // TODO:
}

void CheckerVisitor::applyAssignEqToScope(const BinExpr *bin) {
  assert(TO_PLUSEQ <= bin->op() && bin->op() <= TO_MODEQ);

  const Expr *lhs = bin->lhs();
  const Expr *rhs = bin->rhs();

  const NameRef *name = computeNameRef(lhs);
  if (!name)
    return;

  ValueRef *v = findValueInScopes(name);

  if (v) {
    if (currectInfo) {
      currectInfo->addModifiable(name, v->owner);
    }
    v->kill();
  }
}

void CheckerVisitor::applyBinaryToScope(const BinExpr *bin) {
  switch (bin->op())
  {
  case TO_ASSIGN:
  case TO_INEXPR_ASSIGN:
  case TO_NEWSLOT:
    return applyAssignmentToScope(bin);
    //return applyNewslotToScope(bin);
  case TO_PLUSEQ:
  case TO_MINUSEQ:
  case TO_MULEQ:
  case TO_DIVEQ:
  case TO_MODEQ:
    return applyAssignEqToScope(bin);
  default:
    break;
  }
}

const NameRef *CheckerVisitor::computeNameRef(const Expr *lhs) {
  switch (lhs->op())
  {
  case TO_GETFIELD: return computeNameRef(lhs->asGetField());
  case TO_GETTABLE: return computeNameRef(lhs->asGetTable());
  case TO_ID: return makeNameRef(nullptr, lhs->asId()->id());
  case TO_ROOT: return &rootRef;
  case TO_BASE: return &baseRef;
  // TODO:
  default:
    return nullptr;
  }
}

void CheckerVisitor::setExpression(const Expr *lvalue, const Expr *rvalue, unsigned pf, unsigned nf) {
  const NameRef *name = computeNameRef(lvalue);

  if (!name)
    return;

  ValueRef *v = findValueInScopes(name);

  if (v) {
    v->expression = rvalue;
    v->kind = VRK_EXPRESSION;

    v->flagsPositive |= pf;
    v->flagsPositive &= ~nf;
    v->flagsNegative |= nf;
    v->flagsNegative &= ~pf;
  }
}

const ValueRef *CheckerVisitor::findValueForExpr(const Expr *e) {
  e = deparen(e);
  const NameRef *n = computeNameRef(e);

  if (!n) {
    return nullptr;
  }

  return findValueInScopes(n);
}

bool CheckerVisitor::isPotentiallyNullable(const Expr *e) {
  if (e->op() == TO_LITERAL) {
    return e->asLiteral()->kind() == LK_NULL;
  }

  if (e->isAccessExpr()) {
    if (e->asAccessExpr()->isNullable()) {
      return true;
    }
  }

  if (e->op() == TO_CALL) {
    const CallExpr *call = static_cast<const CallExpr *>(e);
    if (call->isNullable()) {
      return true;
    }
  }

  const ValueRef *v = findValueForExpr(e);
  if (v) {
    if (v->flagsPositive & RT_NULL) {
      return true;
    }

    if (v->flagsNegative & RT_NULL) {
      return false;
    }

    switch (v->kind)
    {
    case VRK_EXPRESSION:
      return isPotentiallyNullable(v->expression);
    default:
      return false;
    }
  }

  return false;
}

const Expr *CheckerVisitor::maybeEval(const Expr *e) {
  e = deparen(e);
  const ValueRef *v = findValueForExpr(e);

  if (!v) {
    return e;
  }

  if (v->kind == VRK_EXPRESSION) {
    return maybeEval(v->expression);
  }
  else {
    return e;
  }
}

const NameRef *CheckerVisitor::computeNameRef(const GetTableExpr *access) {

  const Expr *receiver = access->receiver();
  assert(receiver);

  const NameRef *receiverRef = computeNameRef(receiver);

  if (!receiverRef)
    return nullptr;

  const Expr *key = access->key();
  const SQChar *id = nullptr;


  switch (key->op())
  {
  case TO_LITERAL: {
    const LiteralExpr *l = key->asLiteral();
    if (l->kind() == LK_STRING)
      id = l->s();
    break;
  }
  
  default:
    // TODO: maybe try to resolve key to literal
    break;
  }

  return id ? makeNameRef(receiverRef, id) : nullptr;
}

const NameRef *CheckerVisitor::computeNameRef(const GetFieldExpr *access) {

  const Expr *receiver = access->receiver();
  assert(receiver);

  const NameRef *receiverRef = computeNameRef(receiver);

  if (receiverRef) {
    return makeNameRef(receiverRef, access->fieldName());
  }

  return nullptr;
}

bool CheckerVisitor::isOwnedSymbol(const NameRef *name) {
  auto it = currentScope->symbols.find(name);
  return it != currentScope->symbols.end();
}

void CheckerVisitor::applyKnownInvocationToScope(const ValueRef *value) {
  const FunctionInfo *info = nullptr;

  if (value->kind == VRK_EXPRESSION) {
    const Expr *expr = maybeEval(value->expression);
    assert(expr != nullptr);

    if (expr->op() == TO_DECL_EXPR) {
      const Decl *decl = static_cast<const DeclExpr *>(expr)->declaration();
      if (decl->op() == TO_FUNCTION || decl->op() == TO_CONSTRUCTOR) {
        info = functionInfoMap[static_cast<const FunctionDecl *>(decl)];
      }
      else if (decl->op() == TO_CLASS) {
        const ClassDecl *klass = static_cast<const ClassDecl *>(decl);
        for (auto &m : klass->members()) {
          const Expr *me = m.value;
          if (me->op() == TO_DECL_EXPR) {
            const Decl *de = static_cast<const DeclExpr *>(m.value)->declaration();
            if (de->op() == TO_CONSTRUCTOR) {
              info = functionInfoMap[static_cast<const FunctionDecl *>(de)];
              break;
            }
          }
        }
      }
      else {
        applyUnknownInvocationToScope();
        return;
      }
    }
  }

  if (!info) {
    // probably it is class constructor
    return;
  }

  for (auto s : info->modifible) {
    VarScope *scope = currentScope->findScope(s.owner);
    if (!scope)
      continue;

    auto it = scope->symbols.find(s.name);
    if (it != currentScope->symbols.end()) {
      if (currectInfo) {
        currectInfo->addModifiable(it->first, it->second->owner);
      }
      it->second->kill();
    }
  }
}

void CheckerVisitor::applyUnknownInvocationToScope() {
  VarScope *s = currentScope;

  while (s) {
    auto &symbols = s->symbols;
    for (auto &sym : symbols) {
      if (currectInfo) {
        currectInfo->addModifiable(sym.first, sym.second->owner);
      }
      sym.second->kill();
    }
    s = s->parent;
  }
}

void CheckerVisitor::applyCallToScope(const CallExpr *call) {
  const Expr *callee = deparen(call->callee());

  if (callee->op() == TO_ID) {
    const Id *calleeId = callee->asId();
    const NameRef ref(nullptr, calleeId->id());
    const ValueRef *value = findValueInScopes(&ref);
    if (value) {
      applyKnownInvocationToScope(value);
    }
    else {
      // unknown invocation by pure Id points to some external
      // function which could not modify any scoped value
    }
  }
  else if (callee->op() == TO_GETFIELD) {
    const NameRef *ref = computeNameRef(callee);
    const ValueRef *value = findValueInScopes(ref);
    if (value) {
      applyKnownInvocationToScope(value);
    }
    else if (ref->receiver != &rootRef) {
      // we don't know what exactly is being called so assume the most conservative case
      applyUnknownInvocationToScope();
    }
  }
  else {
    // unknown invocation so everything could be modified
    applyUnknownInvocationToScope();
  }
}

//FunctionInfo *CheckerVisitor::getOrCreateFunctionInfo(const NameRef *name, const FunctionDecl *decl) {
//  FunctionInfo *info = functionInfoMap[decl];
//  if (info)
//    return info;
//
//  void *mem = arena->allocate(sizeof FunctionInfo);
//  info = new(mem) FunctionInfo(name, decl, currentScope->owner);
//  functionInfoMap[decl] = info;
//
//  return info;
//}

void CheckerVisitor::pushFunctionScope(VarScope *functionScope, const FunctionDecl *decl) {

  FunctionInfo *info = functionInfoMap[decl];

  if (!info) {
    info = makeFunctionInfo(makeNameRef(nullptr, decl->name()), decl, currentScope->owner);
    functionInfoMap[decl] = info;
  }

  currentScope = functionScope;

  info->parameters.push_back("this");
  declareSymbol(&thisRef, nullptr, SK_PARAM, decl, nullptr);

  for (auto param : decl->parameters()) {
    info->parameters.push_back(param->name());
    declareSymbol(makeNameRef(nullptr, param->name()), nullptr, SK_PARAM, decl, param);
  }
}

void CheckerVisitor::declareSymbol(const NameRef *nameRef, const Expr *init, enum SymbolKind symkind, const FunctionDecl *owner, Decl *decl) {

  VarScope *current = currentScope;

  ValueRef *ref = makeValueRef();
  ref->symKind = symkind;
  ref->declaration = decl;
  ref->owner = owner;
  ref->expression = init;
  ref->kind = init ? VRK_EXPRESSION : VRK_UNDEFINED;
  ref->flagsPositive = ref->flagsNegative = 0;
  ref->lowerBound.kind = ref->upperBound.kind = VBK_UNKNOWN;

  current->symbols[nameRef] = ref;
}

void CheckerVisitor::visitVarDecl(VarDecl *decl) {

  declareSymbol(makeNameRef(nullptr, decl->name()), decl->initializer(), decl->isAssignable() ? SK_VAR : SK_BINDING, currentScope->owner, decl);

  Visitor::visitVarDecl(decl);
}

void CheckerVisitor::visitConstDecl(ConstDecl *cnst) {
  declareSymbol(makeNameRef(nullptr, cnst->name()), cnst->value(), SK_CONST, currentScope->owner, cnst);

  Visitor::visitConstDecl(cnst);
}

void CheckerVisitor::visitEnumDecl(EnumDecl *enm) {
  const NameRef *enumName = makeNameRef(nullptr, enm->name());
  for (auto &c : enm->consts()) {
    const NameRef *idName = makeNameRef(nullptr, c.id);
    const NameRef *fqnName = makeNameRef(enumName, c.id);
    declareSymbol(idName, c.val, SK_CONST, currentScope->owner, enm);
    declareSymbol(fqnName, c.val, SK_CONST, currentScope->owner, enm);
    c.val->visit(this);
  }
}

void CheckerVisitor::analyse(RootBlock *root) {
  VarScope rootScope(nullptr);
  currentScope = &rootScope;
  root->visit(this);
  currentScope = nullptr;
}

void StaticAnalyser::runAnalysis(RootBlock *root)
{
  CheckerVisitor(_ctx).analyse(root);
}

}
