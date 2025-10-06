#pragma once
#include <cstdio> // for snprintf
#include "sqio.h"
#include "ast.h"

namespace SQCompilation {

class IndentedTreeRenderer : public Visitor {
    OutputStream* _out;
    SQInteger _indent;
    sqvector<int> indents;
    sqvector<int> vertLines;
    bool collectIndentsPass;
    int lineNum;

    void indent() {
        if (collectIndentsPass) {
            indents.push_back(_indent);
            return;
        }

        while (vertLines.size() <= _indent)
            vertLines.push_back(0);

        if (_indent > 0) {
            if (lineNum == 0 || (_indent > indents[lineNum - 1])) {
                int breakPos = lineNum;
                for (int i = lineNum; i < indents.size(); i++) {
                    if (indents[i] == _indent)
                        breakPos = i;
                    if (indents[i] < _indent)
                        break;
                }
                vertLines[_indent - 1] = breakPos - lineNum + 1;
                if (vertLines[_indent - 1] == 1)
                    vertLines[_indent - 1] = 0;
            }

            for (SQInteger i = 0; i < _indent; ++i) {
                if (vertLines[i] > 1)
                    _out->writeString(i == _indent - 1 ? "|-" : "| ");
                else if (vertLines[i] == 1)
                    _out->writeString("`-");
                else
                    _out->writeString("  ");

                if (vertLines[i] > 0)
                    vertLines[i]--;
            }
        }
        lineNum++;
    }

    const char* treeopToStr(TreeOp op) {
        switch (op) {
        case TO_NULLC: return "??";
        case TO_ASSIGN: return "=";
        case TO_OROR: return "||";
        case TO_ANDAND: return "&&";
        case TO_OR: return "|";
        case TO_XOR: return "^";
        case TO_AND: return "&";
        case TO_NE: return "!=";
        case TO_EQ: return "==";
        case TO_3CMP: return "<=>";
        case TO_GE: return ">=";
        case TO_GT: return ">";
        case TO_LE: return "<=";
        case TO_LT: return "<";
        case TO_IN: return "IN";
        case TO_INSTANCEOF: return "INSTANCEOF";
        case TO_USHR: return ">>>";
        case TO_SHR: return ">>";
        case TO_SHL: return "<<";
        case TO_MUL: return "*";
        case TO_DIV: return "/";
        case TO_MOD: return "%";
        case TO_ADD: return "+";
        case TO_SUB: case TO_NEG: return "-";
        case TO_NOT: return "!";
        case TO_BNOT: return "~";
        case TO_TYPEOF: return "TYPEOF";
        case TO_RESUME: return "RESUME";
        case TO_CLONE: return "CLONE";
        case TO_DELETE: return "DELETE";
        case TO_NEWSLOT: return "<-";
        case TO_PLUSEQ: return "+=";
        case TO_MINUSEQ: return "-=";
        case TO_MULEQ: return "*=";
        case TO_DIVEQ: return "/=";
        case TO_MODEQ: return "%=";
        case TO_STATIC_MEMO: return "STATIC_MEMO";
        case TO_INLINE_CONST: return "INLINE_CONST";
        case TO_PAREN: return "(";
        default: return "<UNKNOWN_OP>";
        }
    }

    void writeFmtString(const char* fmt, ...) {
        if (collectIndentsPass)
            return;
        char buf[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        _out->writeString(buf);
    }

    void writeIndentedFmtString(const char* fmt, ...) {
        indent();
        if (collectIndentsPass)
            return;
        char buf[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        _out->writeString(buf);
    }

public:
    IndentedTreeRenderer(OutputStream* output) : _out(output), _indent(0), indents(nullptr), vertLines(nullptr),
        collectIndentsPass(false), lineNum(0) {}

    void render(Node* n) {
        if (n) {
            collectIndentsPass = true;
            n->visit(this);
            collectIndentsPass = false;
            n->visit(this);
        } else {
            indent();
            _out->writeString("(null)");
        }
    }

    virtual void visitNode(Node* node) override {
        TreeOp op = node->op();

        switch (op) {
            case TO_ID: {
                Id* id = static_cast<Id*>(node);
                writeIndentedFmtString("Id '%s'\n", id->name());
                break;
            }

            case TO_LITERAL: {
                LiteralExpr* lit = static_cast<LiteralExpr*>(node);
                switch (lit->kind()) {
                    case LK_STRING:
                        writeIndentedFmtString("LiteralExpr LK_STRING \"%s\"\n", lit->s());
                        break;
                    case LK_FLOAT:
                        writeIndentedFmtString("LiteralExpr LK_FLOAT %f\n", lit->f());
                        break;
                    case LK_INT:
                        writeIndentedFmtString("LiteralExpr LK_INT %lld\n", (long long)lit->i());
                        break;
                    case LK_BOOL:
                        writeIndentedFmtString("LiteralExpr LK_BOOL %s\n", lit->b() ? "true" : "false");
                        break;
                    case LK_NULL:
                        writeIndentedFmtString("LiteralExpr LK_NULL\n");
                        break;
                    default:
                        writeIndentedFmtString("LiteralExpr <UNKNOWN_LITERAL>\n");
                        break;
                }
                break;
            }

            case TO_NOT:
            case TO_BNOT:
            case TO_NEG:
            case TO_TYPEOF:
            case TO_RESUME:
            case TO_CLONE:
            case TO_PAREN:
            case TO_DELETE:
            case TO_STATIC_MEMO:
            case TO_INLINE_CONST: {
                UnExpr* expr = static_cast<UnExpr*>(node);
                writeIndentedFmtString("UnExpr '%s'\n", treeopToStr(expr->op()));
                ++_indent;
                expr->argument()->visit(this);
                --_indent;
                break;
            }

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
            case TO_PLUSEQ:
            case TO_MINUSEQ:
            case TO_MULEQ:
            case TO_DIVEQ:
            case TO_MODEQ:
            {
                BinExpr* expr = static_cast<BinExpr*>(node);
                writeIndentedFmtString("BinExpr '%s'\n", treeopToStr(expr->op()));
                ++_indent;
                writeIndentedFmtString("Left\n");
                ++_indent;
                expr->_lhs->visit(this);
                --_indent;
                writeIndentedFmtString("Right\n");
                ++_indent;
                expr->_rhs->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_TERNARY: {
                TerExpr* expr = static_cast<TerExpr*>(node);
                writeIndentedFmtString("TerExpr\n");
                ++_indent;
                writeIndentedFmtString("Condition\n");
                ++_indent;
                expr->a()->visit(this);
                --_indent;
                writeIndentedFmtString("TrueBranch\n");
                ++_indent;
                expr->b()->visit(this);
                --_indent;
                writeIndentedFmtString("FalseBranch\n");
                ++_indent;
                expr->c()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_CALL: {
                CallExpr* expr = static_cast<CallExpr*>(node);
                writeIndentedFmtString("CallExpr\n");
                ++_indent;
                writeIndentedFmtString("Callee\n");
                ++_indent;
                expr->callee()->visit(this);
                --_indent;
                --_indent;
                if (!expr->arguments().empty()) {
                    ++_indent;
                    writeIndentedFmtString("Arguments\n");
                    ++_indent;
                    for (int i = 0; i < (int)expr->arguments().size(); ++i) {
                        writeIndentedFmtString("Argument #%d\n", i + 1);
                        ++_indent;
                        expr->arguments()[i]->visit(this);
                        --_indent;
                    }
                    --_indent;
                    --_indent;
                }
                break;
            }

            case TO_GETFIELD: {
                GetFieldExpr* expr = static_cast<GetFieldExpr*>(node);
                writeIndentedFmtString("GetFieldExpr '%s' fieldName = '%s'\n", expr->isNullable() ? "?.":".", expr->fieldName());
                ++_indent;
                writeIndentedFmtString("Receiver\n");
                ++_indent;
                expr->receiver()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_SETFIELD: {
                SetFieldExpr* expr = static_cast<SetFieldExpr*>(node);
                writeIndentedFmtString("SetFieldExpr '%s' fieldName = '%s'\n", expr->isNullable() ? "?." : ".", expr->fieldName());
                ++_indent;
                writeIndentedFmtString("Receiver\n");
                ++_indent;
                expr->receiver()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_GETSLOT: {
                GetSlotExpr* expr = static_cast<GetSlotExpr*>(node);
                writeIndentedFmtString("GetSlotExpr '%s'\n", expr->isNullable() ? "?[" : "[");
                ++_indent;
                writeIndentedFmtString("Receiver\n");
                ++_indent;
                expr->receiver()->visit(this);
                --_indent;
                writeIndentedFmtString("Key\n");
                ++_indent;
                expr->key()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_SETSLOT: {
                SetSlotExpr* expr = static_cast<SetSlotExpr*>(node);
                writeIndentedFmtString("SetSlotExpr '%s'\n", expr->isNullable() ? "?[" : "[");
                ++_indent;
                writeIndentedFmtString("Receiver\n");
                ++_indent;
                expr->receiver()->visit(this);
                --_indent;
                writeIndentedFmtString("Key\n");
                ++_indent;
                expr->key()->visit(this);
                --_indent;
                writeIndentedFmtString("Value\n");
                ++_indent;
                expr->value()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_INC: {
                IncExpr* expr = static_cast<IncExpr*>(node);
                writeIndentedFmtString("%s '%s'\n",
                    (expr->form() == IF_PREFIX) ? "IncExpr IF_PREFIX" : "IncExpr IF_POSTFIX", (expr->diff() < 0) ? "--" : "++");
                ++_indent;
                expr->argument()->visit(this);
                --_indent;
                break;
            }

            case TO_ARRAYEXPR: {
                ArrayExpr* expr = static_cast<ArrayExpr*>(node);
                writeIndentedFmtString("ArrayExpr\n");
                if (!expr->initializers().empty()) {
                    ++_indent;
                    for (int i = 0; i < (int)expr->initializers().size(); ++i) {
                        writeIndentedFmtString("Element #%d\n", i + 1);
                        ++_indent;
                        expr->initializers()[i]->visit(this);
                        --_indent;
                    }
                    --_indent;
                }
                break;
            }

            case TO_COMMA: {
                CommaExpr* expr = static_cast<CommaExpr*>(node);
                writeIndentedFmtString("CommaExpr\n");
                ++_indent;
                for (int i = 0; i < (int)expr->expressions().size(); ++i) {
                    writeIndentedFmtString("Expression #%d\n", i + 1);
                    ++_indent;
                    expr->expressions()[i]->visit(this);
                    --_indent;
                }
                --_indent;
                break;
            }


            case TO_BLOCK: {
                Block* block = static_cast<Block*>(node);
                writeIndentedFmtString("Block isRoot = %d, isBody = %d\n", int(block->isRoot()), int(block->isBody()));
                ++_indent;
                for (Statement* stmt : block->statements()) {
                    stmt->visit(this);
                }
                --_indent;
                break;
            }

            case TO_IF: {
                IfStatement* stmt = static_cast<IfStatement*>(node);
                writeIndentedFmtString("IfStatement\n");
                ++_indent;
                writeIndentedFmtString("Condition\n");
                ++_indent;
                stmt->condition()->visit(this);
                --_indent;
                writeIndentedFmtString("ThenBranch\n");
                ++_indent;
                stmt->thenBranch()->visit(this);
                --_indent;
                if (stmt->elseBranch()) {
                    writeIndentedFmtString("ElseBranch\n");
                    ++_indent;
                    stmt->elseBranch()->visit(this);
                    --_indent;
                }
                --_indent;
                break;
            }

            case TO_WHILE: {
                WhileStatement* loop = static_cast<WhileStatement*>(node);
                writeIndentedFmtString("WhileStatement\n");
                ++_indent;
                writeIndentedFmtString("Condition\n");
                ++_indent;
                loop->condition()->visit(this);
                --_indent;
                writeIndentedFmtString("Body\n");
                ++_indent;
                loop->body()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_DOWHILE: {
                DoWhileStatement* loop = static_cast<DoWhileStatement*>(node);
                writeIndentedFmtString("DoWhileStatement\n");
                ++_indent;
                writeIndentedFmtString("Body\n");
                ++_indent;
                loop->body()->visit(this);
                --_indent;
                writeIndentedFmtString("Condition\n");
                ++_indent;
                loop->condition()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_FOR: {
                ForStatement* loop = static_cast<ForStatement*>(node);
                writeIndentedFmtString("ForStatement\n");
                ++_indent;
                if (loop->initializer()) {
                    writeIndentedFmtString("Initializer\n");
                    ++_indent;
                    loop->initializer()->visit(this);
                    --_indent;
                }
                if (loop->condition()) {
                    writeIndentedFmtString("Condition\n");
                    ++_indent;
                    loop->condition()->visit(this);
                    --_indent;
                }
                if (loop->modifier()) {
                    writeIndentedFmtString("Modifier\n");
                    ++_indent;
                    loop->modifier()->visit(this);
                    --_indent;
                }
                writeIndentedFmtString("Body\n");
                ++_indent;
                loop->body()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_FOREACH: {
                ForeachStatement* loop = static_cast<ForeachStatement*>(node);
                writeIndentedFmtString("ForeachStatement\n");
                ++_indent;
                if (loop->idx()) {
                    writeIndentedFmtString("IndexVariable\n");
                    ++_indent;
                    loop->idx()->visit(this);
                    --_indent;
                }
                writeIndentedFmtString("ValueVariable\n");
                ++_indent;
                loop->val()->visit(this);
                --_indent;
                writeIndentedFmtString("Container\n");
                ++_indent;
                loop->container()->visit(this);
                --_indent;
                writeIndentedFmtString("Body\n");
                ++_indent;
                loop->body()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_TRY: {
                TryStatement* tr = static_cast<TryStatement*>(node);
                writeIndentedFmtString("TryStatement\n");
                ++_indent;
                writeIndentedFmtString("TryBlock\n");
                ++_indent;
                tr->tryStatement()->visit(this);
                --_indent;
                writeIndentedFmtString("CatchBlock exceptionId = '%s'\n", tr->exceptionId()->name());
                ++_indent;
                tr->catchStatement()->visit(this);
                --_indent;
                --_indent;
                break;
            }

            case TO_RETURN: {
                ReturnStatement* ret = static_cast<ReturnStatement*>(node);
                writeIndentedFmtString("ReturnStatement\n");
                if (ret->argument()) {
                    ++_indent;
                    ret->argument()->visit(this);
                    --_indent;
                }
                break;
            }

            case TO_THROW: {
                ThrowStatement* thr = static_cast<ThrowStatement*>(node);
                writeIndentedFmtString("ThrowStatement\n");
                if (thr->argument()) {
                    ++_indent;
                    thr->argument()->visit(this);
                    --_indent;
                }
                break;
            }

            case TO_YIELD: {
                YieldStatement* yield = static_cast<YieldStatement*>(node);
                writeIndentedFmtString("YieldStatement\n");
                if (yield->argument()) {
                    ++_indent;
                    yield->argument()->visit(this);
                    --_indent;
                }
                break;
            }

            case TO_BREAK: {
                writeIndentedFmtString("BreakStatement\n");
                break;
            }

            case TO_CONTINUE: {
                writeIndentedFmtString("ContinueStatement\n");
                break;
            }

            case TO_EMPTY: {
                writeIndentedFmtString("EmptyStatement\n");
                break;
            }

            case TO_DIRECTIVE: {
                DirectiveStmt* directive = static_cast<DirectiveStmt*>(node);
                writeIndentedFmtString("DirectiveStmt\n");
                break;
            }

            case TO_BASE: {
                writeIndentedFmtString("BaseExpr\n");
                break;
            }

            case TO_EXPR_STMT: {
                ExprStatement* estmt = static_cast<ExprStatement*>(node);
                writeIndentedFmtString("ExprStatement\n");
                ++_indent;
                estmt->expression()->visit(this);
                --_indent;
                break;
            }

            case TO_DECL_EXPR: {
                DeclExpr* declExpr = static_cast<DeclExpr*>(node);
                writeIndentedFmtString("DeclExpr\n");
                ++_indent;
                declExpr->declaration()->visit(this);
                --_indent;
                break;
            }

            case TO_VAR: {
                VarDecl* decl = static_cast<VarDecl*>(node);
                const char* kind = decl->isAssignable() ? "VarDecl 'local'" : "VarDecl 'let'";
                writeIndentedFmtString("%s '%s'\n", kind, decl->name());
                if (decl->expression()) {
                    ++_indent;
                    writeIndentedFmtString("Initializer\n");
                    ++_indent;
                    decl->expression()->visit(this);
                    --_indent;
                    --_indent;
                }
                break;
            }

            case TO_DECL_GROUP: {
                DeclGroup* dgrp = static_cast<DeclGroup*>(node);
                writeIndentedFmtString("DeclGroup\n");
                ++_indent;
                for (auto& decl : dgrp->declarations()) {
                    decl->visit(this);
                }
                --_indent;
                break;
            }

            case TO_ROOT_TABLE_ACCESS: {
                writeIndentedFmtString("RootTableAccessExpr\n");
                break;
            }

            case TO_DESTRUCTURE: {
                DestructuringDecl* dstr = static_cast<DestructuringDecl*>(node);
                writeIndentedFmtString("DestructuringDecl, type = '%s'\n",
                    dstr->type() == DT_ARRAY ? "array" : (dstr->type() == DT_TABLE ? "table" : "unknown"));
                ++_indent;
                if (dstr->initExpression()) {
                    writeIndentedFmtString("Initializer\n");
                    ++_indent;
                    dstr->initExpression()->visit(this);
                    --_indent;
                }
                for (auto& decl : dstr->declarations()) {
                    decl->visit(this);
                }
                --_indent;
                break;
            }

            case TO_CONST: {
                ConstDecl* constDecl = static_cast<ConstDecl*>(node);
                writeIndentedFmtString("ConstDecl name = '%s', isGlobal = %d\n", constDecl->name(), int(constDecl->isGlobal()));
                ++_indent;
                if (constDecl->value()) {
                    constDecl->value()->visit(this);
                } else {
                    indent();
                    _out->writeString("(null)\n");
                }
                --_indent;
                break;
            }

            case TO_ENUM: {
                EnumDecl* enm = static_cast<EnumDecl*>(node);
                writeIndentedFmtString("EnumDecl name = '%s', isGlobal = %d\n", enm->name(), int(enm->isGlobal()));
                ++_indent;
                for (auto& c : enm->consts()) {
                    writeIndentedFmtString("Member '%s'\n", c.id);
                    ++_indent;
                    if (c.val) {
                        c.val->visit(this);
                    } else {
                        indent();
                        _out->writeString("(null)\n");
                    }
                    --_indent;
                }
                --_indent;
                break;
            }

            case TO_TABLE: {
                TableDecl* tbl = static_cast<TableDecl*>(node);
                writeIndentedFmtString("TableDecl\n");
                ++_indent;
                for (auto& m : tbl->members()) {
                    writeIndentedFmtString("Field\n");
                    ++_indent;
                    writeIndentedFmtString("Key\n");
                    ++_indent;
                    m.key->visit(this);
                    --_indent;
                    writeIndentedFmtString("Value\n");
                    ++_indent;
                    m.value->visit(this);
                    --_indent;
                    --_indent;
                }
                --_indent;
                break;
            }

            case TO_CLASS: {
                ClassDecl* cls = static_cast<ClassDecl*>(node);
                writeIndentedFmtString("ClassDecl\n");
                if (cls->classKey()) {
                    ++_indent;
                    writeIndentedFmtString("Key\n");
                    ++_indent;
                    cls->classKey()->visit(this);
                    --_indent;
                    --_indent;
                } else {
                    ++_indent;
                    writeIndentedFmtString("Key: <anonymous>\n");
                    --_indent;
                }

                if (cls->classBase()) {
                    ++_indent;
                    writeIndentedFmtString("Base\n");
                    ++_indent;
                    cls->classBase()->visit(this);
                    --_indent;
                    --_indent;
                }

                ++_indent;
                writeIndentedFmtString("Members\n");
                ++_indent;
                for (auto& member : cls->members()) {
                    member.value->visit(this);
                }
                --_indent;
                --_indent;
                break;
            }

            case TO_CONSTRUCTOR:
            case TO_FUNCTION: {
                FunctionDecl* fn = static_cast<FunctionDecl*>(node);
                writeIndentedFmtString("%s name = '%s'\n",
                    op == TO_CONSTRUCTOR ? "ConstructorDecl" : fn->isLambda() ? "FunctionDecl 'lambda'" : "FunctionDecl",
                    fn->name() ? fn->name() : "<anonymous>");

                ++_indent;
                writeIndentedFmtString("Parameters count = %d\n", (int)fn->parameters().size());
                ++_indent;
                for (int i = 0; i < (int)fn->parameters().size(); ++i) {
                    writeIndentedFmtString("Parameter #%d name = '%s'%s%s\n", i + 1, fn->parameters()[i]->name(),
                        fn->parameters()[i]->isVararg() ? " (vararg)" : "", fn->parameters()[i]->hasDefaultValue() ? " (has default)" : "");
                    if (fn->parameters()[i]->hasDefaultValue()) {
                        ++_indent;
                        writeIndentedFmtString("DefaultValue\n");
                        ++_indent;
                        fn->parameters()[i]->defaultValue()->visit(this);
                        --_indent;
                        --_indent;
                    }
                }

                if (fn->isVararg()) {
                    writeIndentedFmtString("Vararg...\n");
                }

                --_indent;
                if (fn->body()) {
                    writeIndentedFmtString("Body\n");
                    ++_indent;
                    fn->body()->visit(this);
                    --_indent;
                } else {
                    writeIndentedFmtString("Body: <null>\n");
                }

                --_indent;
                break;
            }

            case TO_IMPORT: {
                ImportStmt* imp = static_cast<ImportStmt*>(node);
                writeIndentedFmtString("ImportStmt module '%s'", imp->moduleName);
                if (imp->moduleAlias)
                    writeFmtString(" as '%s'", imp->moduleAlias);
                writeFmtString("\n");

                if (!imp->slots.empty()) {
                    ++_indent;
                    for (const SQModuleImportSlot& slot : imp->slots) {
                        writeIndentedFmtString("'%s'", slot.name);
                        if (slot.alias)
                            writeFmtString(" as '%s'", slot.alias);
                        writeFmtString("\n");
                    }
                    --_indent;
                }
                break;
            }

            default: {
                writeIndentedFmtString("ERROR: Unknown node type, op = %d\n", (int)op);
                break;
            }
        }
    }
};

} // namespace SQCompilation
