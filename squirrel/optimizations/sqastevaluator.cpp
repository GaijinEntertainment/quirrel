#ifndef NO_COMPILER
#include <math.h>
#include "sqastevaluator.h"
#include "../sqastcodegen.h"

namespace SQCompilation {

template<typename E>
static E *copyCoordinates(const Expr *from, E *to) {
  to->setLineStartPos(from->lineStart());
  to->setColumnStartPos(from->columnStart());
  to->setLineEndPos(from->lineEnd());
  to->setColumnEndPos(from->columnEnd());

  return to;
}

LiteralExpr *ExpressionEvaluator::evalNullCExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_NULLC);

  if (l->kind() == LK_NULL)
    return copyCoordinates(orig, const_cast<LiteralExpr *>(r));
  return copyCoordinates(orig, const_cast<LiteralExpr *>(l));
  return nullptr;
}

LiteralExpr *ExpressionEvaluator::evalOrOrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_OROR);

  switch (l->kind())
  {
  case LK_STRING: return copyCoordinates(orig, const_cast<LiteralExpr *>(l));
  case LK_FLOAT: return copyCoordinates(orig, const_cast<LiteralExpr *>(l->f() != 0.0f ? l : r));
  case LK_INT: return copyCoordinates(orig, const_cast<LiteralExpr *>(l->i() != 0 ? l : r));
  case LK_BOOL: return copyCoordinates(orig, const_cast<LiteralExpr *>(l->b() ? l : r));
  case LK_NULL: return copyCoordinates(orig, const_cast<LiteralExpr *>(r));
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalAndAndExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_ANDAND);

  switch (l->kind())
  {
  case LK_STRING: return copyCoordinates(orig, const_cast<LiteralExpr *>(r));
  case LK_FLOAT: return copyCoordinates(orig, const_cast<LiteralExpr *>(l->f() != 0.0f ? r : l));
  case LK_INT: return copyCoordinates(orig, const_cast<LiteralExpr *>(l->i() != 0 ? r : l));
  case LK_BOOL: return copyCoordinates(orig, const_cast<LiteralExpr *>(l->b() ? r : l));
  case LK_NULL: return copyCoordinates(orig, const_cast<LiteralExpr *>(l));
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalOrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_OR);

  if (l->kind() != LK_INT || r->kind() != LK_INT)
    return nullptr;

  return copyCoordinates(orig, newNode<LiteralExpr>(l->i() | r->i()));
}

LiteralExpr *ExpressionEvaluator::evalXorOrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_XOR);

  if (l->kind() != LK_INT || r->kind() != LK_INT)
    return nullptr;

  return copyCoordinates(orig, newNode<LiteralExpr>(l->i() ^ r->i()));
}

LiteralExpr *ExpressionEvaluator::evalAndExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_AND);

  if (l->kind() != LK_INT || r->kind() != LK_INT)
    return nullptr;

  return copyCoordinates(orig, newNode<LiteralExpr>(l->i() & r->i()));
}

LiteralExpr *ExpressionEvaluator::evalNeExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_NE);
  if (l->kind() != r->kind())
    return copyCoordinates(orig, newNode<LiteralExpr>(true));

  switch (l->kind())
  {
  case LK_STRING: return copyCoordinates(orig, newNode<LiteralExpr>(strcmp(l->s(), r->s()) != 0));
  case LK_FLOAT: return copyCoordinates(orig, newNode<LiteralExpr>(l->f() != r->f()));
  case LK_INT: return copyCoordinates(orig, newNode<LiteralExpr>(l->i() != r->i()));
  case LK_BOOL: return copyCoordinates(orig, newNode<LiteralExpr>(l->b() != r->b()));
  case LK_NULL: return copyCoordinates(orig, newNode<LiteralExpr>(false));
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalEqExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_EQ);
  if (l->kind() != r->kind())
    return copyCoordinates(orig, newNode<LiteralExpr>(true));

  switch (l->kind())
  {
  case LK_STRING: return copyCoordinates(orig, newNode<LiteralExpr>(strcmp(l->s(), r->s()) == 0));
  case LK_FLOAT: return copyCoordinates(orig, newNode<LiteralExpr>(l->f() == r->f()));
  case LK_INT: return copyCoordinates(orig, newNode<LiteralExpr>(l->i() == r->i()));
  case LK_BOOL: return copyCoordinates(orig, newNode<LiteralExpr>(l->b() == r->b()));
  case LK_NULL: return copyCoordinates(orig, newNode<LiteralExpr>(true));
  default: return nullptr;
  }
}

static bool compareLiterals(const LiteralExpr *l, const LiteralExpr *r, int32_t &result) {
  if (l->kind() == r->kind()) {
    switch (l->kind()) {
    case LK_STRING: result = strcmp(l->s(), r->s()); return true;
    case LK_FLOAT: result = (l->f() == r->f()) ? 0 :(l->f() < r->f() ? -1 : 1); return true;
    case LK_INT:  result = (l->i() == r->i()) ? 0 : (l->i() < r->i() ? -1 : 1); return true;
    case LK_BOOL: result = (l->b() == r->b()) ? 0 : (l->b() ? 1 : -1); return true;
    case LK_NULL: result = 0; return true;
    default: return false;
    }
  }
  else {
    if (l->kind() == LK_STRING || r->kind() == LK_STRING)
      return false;

    if (l->kind() == LK_BOOL || r->kind() == LK_BOOL)
      return false;

    if (l->kind() == LK_FLOAT) {
      SQFloat f = static_cast<SQFloat>(r->i());
      result = l->f() == f ? 0 : (l->f() < f ? 1 : -1);
    }
    else if (r->kind() == LK_FLOAT) {
      SQFloat f = static_cast<SQFloat>(l->i());
      result = f == r->f() ? 0 : (f < r->f() ? 1 : -1);
    }
    else {
      result = (l->i() == r->i()) ? 0 : (l->i() < r->i() ? -1 : 1);
    }
    return true;
  }
}

LiteralExpr *ExpressionEvaluator::eval3CmpExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_3CMP);

  int32_t result = 0;

  if (compareLiterals(l, r, result)) {
    return copyCoordinates(orig, newNode<LiteralExpr, SQInteger>(result));
  }
  else {
    return nullptr;
  }
  return nullptr;
}

LiteralExpr *ExpressionEvaluator::evalGeExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_GE);

  int32_t result = 0;

  if (compareLiterals(l, r, result)) {
    return copyCoordinates(orig, newNode<LiteralExpr>(result >= 0));
  }
  else {
    return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalGtExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_GT);

  int32_t result = 0;

  if (compareLiterals(l, r, result)) {
    return copyCoordinates(orig, newNode<LiteralExpr>(result > 0));
  }
  else {
    return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalLeExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_LE);

  int32_t result = 0;

  if (compareLiterals(l, r, result)) {
    return copyCoordinates(orig, newNode<LiteralExpr>(result <= 0));
  }
  else {
    return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalLtExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_LT);

  int32_t result = 0;

  if (compareLiterals(l, r, result)) {
    return copyCoordinates(orig, newNode<LiteralExpr>(result < 0));
  }
  else {
    return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalUshrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_USHR);

  if (l->kind() != LK_INT || r->kind() != LK_INT)
    return nullptr;

  SQInteger li = l->i();
  SQUnsignedInteger lui = *(SQUnsignedInteger *)&li;

  return copyCoordinates(orig, newNode<LiteralExpr, SQInteger>(SQInteger(lui >> r->i())));
}

LiteralExpr *ExpressionEvaluator::evalShrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_SHR);

  if (l->kind() != LK_INT || r->kind() != LK_INT)
    return nullptr;

  return copyCoordinates(orig, newNode<LiteralExpr>(l->i() >> r->i()));
}

LiteralExpr *ExpressionEvaluator::evalShlExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_SHL);

  if (l->kind() != LK_INT || r->kind() != LK_INT)
    return nullptr;

  return copyCoordinates(orig, newNode<LiteralExpr, SQInteger>(l->i() << r->i()));
}

LiteralExpr *ExpressionEvaluator::evalMulExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_MUL);
  int32_t mask = l->kind() | r->kind();

  if (mask & ~(LK_FLOAT | LK_INT))
    return nullptr;

  if (l->kind() != r->kind()) {
    SQFloat lf = l->kind() == LK_FLOAT ? l->f() : (SQFloat)l->i();
    SQFloat rf = r->kind() == LK_FLOAT ? r->f() : (SQFloat)r->i();
    return copyCoordinates(orig, newNode<LiteralExpr>(lf * rf));
  }
  else if (l->kind() == LK_FLOAT) {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->f() * r->f()));
  }
  else {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->i() * r->i()));
  }
}

LiteralExpr *ExpressionEvaluator::evalDivExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_DIV);
  int32_t mask = l->kind() | r->kind();

  if (mask & ~(LK_FLOAT | LK_INT))
    return nullptr;

  if (r->raw() == 0)
    return nullptr;

  if (l->kind() != r->kind()) {
    SQFloat lf = l->kind() == LK_FLOAT ? l->f() : (SQFloat)l->i();
    SQFloat rf = r->kind() == LK_FLOAT ? r->f() : (SQFloat)r->i();
    return copyCoordinates(orig, newNode<LiteralExpr>(lf / rf));
  }
  else if (l->kind() == LK_FLOAT) {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->f() / r->f()));
  }
  else {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->i() / r->i()));
  }
}

LiteralExpr *ExpressionEvaluator::evalModExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_MOD);
  int32_t mask = l->kind() | r->kind();

  if (mask & ~(LK_FLOAT | LK_INT))
    return nullptr;

  if (r->raw() == 0)
    return nullptr;

  if (l->kind() != r->kind()) {
    SQFloat lf = l->kind() == LK_FLOAT ? l->f() : (SQFloat)l->i();
    SQFloat rf = r->kind() == LK_FLOAT ? r->f() : (SQFloat)r->i();
    return copyCoordinates(orig, newNode<LiteralExpr>(fmod(lf, rf)));
  }
  else if (l->kind() == LK_FLOAT) {
    return copyCoordinates(orig, newNode<LiteralExpr>(fmod(l->f(), r->f())));
  }
  else {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->i() % r->i()));
  }
}

#ifdef _SQ64
# define SQ_UINT_FMT PRIi64
#else
# define SQ_UINT_FMT PRIiPTR
#endif // _SQ64

#ifdef SQUSEDOUBLE
# define SQ_FLOAT_FMT "lf"
#else
# define SQ_FLOAT_FMT "f"
#endif // SQUSEDOUBLE

static void appendLiteral(std::string &result, const LiteralExpr *lit) {
  SQChar buf[128] = {0};
  int32_t len;

  switch (lit->kind())
  {
  case LK_FLOAT: len = snprintf(buf, sizeof buf, "%" SQ_FLOAT_FMT, lit->f()); break;
  case LK_INT:   len = snprintf(buf, sizeof buf, "%" SQ_UINT_FMT, lit->i()); break;
  case LK_BOOL:  len = snprintf(buf, sizeof buf, "%s", lit->b() ? "true" : "false"); break;
  case LK_NULL:  len = snprintf(buf, sizeof buf, "%s", "null"); break;
  default: break;
  }

  result.append(buf);
}


LiteralExpr *ExpressionEvaluator::evalAddExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_ADD);

  int32_t mask = l->kind() | r->kind();

  if (mask & LK_STRING) {
    const SQChar *value = nullptr;
    std::string tmp;
    if (l->kind() == r->kind()) {
      tmp.append(l->s());
      tmp.append(r->s());
    }
    else {
      if (l->kind() == LK_STRING) {
        tmp.append(l->s());
        appendLiteral(tmp, r);
      }
      else {
        appendLiteral(tmp, l);
        tmp.append(r->s());
      }
    }

    return copyCoordinates(orig, newNode<LiteralExpr>(copyString(tmp.c_str(), tmp.length())));
  }

  if (mask & ~(LK_FLOAT | LK_INT))
    return nullptr;

  if (l->kind() != r->kind()) {
    SQFloat lf = l->kind() == LK_FLOAT ? l->f() : (SQFloat)l->i();
    SQFloat rf = r->kind() == LK_FLOAT ? r->f() : (SQFloat)r->i();
    return copyCoordinates(orig, newNode<LiteralExpr>(lf + rf));
  }
  else if (l->kind() == LK_FLOAT) {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->f() + r->f()));
  }
  else {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->i() + r->i()));
  }
}

LiteralExpr *ExpressionEvaluator::evalSubExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r) {
  assert(orig->op() == TO_SUB);
  int32_t mask = l->kind() | r->kind();

  if (mask & ~(LK_FLOAT | LK_INT))
    return nullptr;

  if (l->kind() != r->kind()) {
    SQFloat lf = l->kind() == LK_FLOAT ? l->f() : (SQFloat)l->i();
    SQFloat rf = r->kind() == LK_FLOAT ? r->f() : (SQFloat)r->i();
    return copyCoordinates(orig, newNode<LiteralExpr>(lf - rf));
  }
  else if (l->kind() == LK_FLOAT) {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->f() - r->f()));
  }
  else {
    return copyCoordinates(orig, newNode<LiteralExpr>(l->i() - r->i()));
  }
}

Expr *ExpressionEvaluator::evalBinArith(BinExpr *bin) {
  Expr *lhs = bin->lhs();
  Expr *rhs = bin->rhs();

  Expr *evalLhs = evaluate(lhs);
  Expr *evalRhs = evaluate(rhs);

  if (!evalLhs || !evalRhs)
    return nullptr;

  LiteralExpr *leftLit = evalLhs->asLiteral();
  LiteralExpr *rightLit = evalRhs->asLiteral();

  switch (bin->op())
  {
  case TO_NULLC: return evalNullCExpr(bin, leftLit, rightLit);
  case TO_OROR: return evalOrOrExpr(bin, leftLit, rightLit);
  case TO_ANDAND: return evalAndAndExpr(bin, leftLit, rightLit);
  case TO_OR: return evalOrExpr(bin, leftLit, rightLit);
  case TO_XOR: return evalXorOrExpr(bin, leftLit, rightLit);
  case TO_AND: return evalAndExpr(bin, leftLit, rightLit);
  case TO_NE: return evalNeExpr(bin, leftLit, rightLit);
  case TO_EQ: return evalEqExpr(bin, leftLit, rightLit);
  case TO_3CMP: return eval3CmpExpr(bin, leftLit, rightLit);
  case TO_GE: return evalGeExpr(bin, leftLit, rightLit);
  case TO_GT: return evalGtExpr(bin, leftLit, rightLit);
  case TO_LE: return evalLeExpr(bin, leftLit, rightLit);
  case TO_LT: return evalLtExpr(bin, leftLit, rightLit);
  case TO_USHR: return evalUshrExpr(bin, leftLit, rightLit);
  case TO_SHR: return evalShrExpr(bin, leftLit, rightLit);
  case TO_SHL: return evalShlExpr(bin, leftLit, rightLit);
  case TO_MUL: return evalMulExpr(bin, leftLit, rightLit);
  case TO_DIV: return evalDivExpr(bin, leftLit, rightLit);
  case TO_MOD: return evalModExpr(bin, leftLit, rightLit);
  case TO_ADD: return evalAddExpr(bin, leftLit, rightLit);
  case TO_SUB: return evalSubExpr(bin, leftLit, rightLit);
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalNot(const UnExpr *orig, const LiteralExpr *arg) {
  assert(orig->op() == TO_NOT);

  switch (arg->kind())
  {
  case LK_STRING: return copyCoordinates(orig, newNode<LiteralExpr>(false));
  case LK_FLOAT: return copyCoordinates(orig, newNode<LiteralExpr>(arg->f() == 0.0));
  case LK_INT: return copyCoordinates(orig, newNode<LiteralExpr>(arg->i() == 0));
  case LK_BOOL: return copyCoordinates(orig, newNode<LiteralExpr>(arg->b()));
  case LK_NULL: return copyCoordinates(orig, newNode<LiteralExpr>(true));
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalBNot(const UnExpr *orig, const LiteralExpr *arg) {
  assert(orig->op() == TO_BNOT);

  switch (arg->kind())
  {
  case LK_INT: return copyCoordinates(orig, newNode<LiteralExpr, SQInteger>(~arg->i()));
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalNeg(const UnExpr *orig, const LiteralExpr *arg) {
  assert(orig->op() == TO_NEG);

  switch (arg->kind())
  {
  case LK_FLOAT: return copyCoordinates(orig, newNode<LiteralExpr>(-arg->f()));
  case LK_INT: return copyCoordinates(orig, newNode<LiteralExpr>(-arg->i()));
  default: return nullptr;
  }
}

LiteralExpr *ExpressionEvaluator::evalTypeof(const UnExpr *orig, const LiteralExpr *arg) {
  assert(orig->op() == TO_TYPEOF);

  const SQChar *typeName = nullptr;

  switch (arg->kind())
  {
  case LK_STRING: typeName = "string"; break;
  case LK_FLOAT: typeName = "float"; break;
  case LK_INT: typeName = "integer"; break;
  case LK_BOOL: typeName = "bool"; break;
  case LK_NULL: typeName = "null"; break;
  default: return nullptr;
  }

  return copyCoordinates(orig, newNode<LiteralExpr>(typeName));
}

Expr *ExpressionEvaluator::evalUnary(UnExpr *u) {

  Expr *evaled = evaluate(u->argument());
  if (evaled == nullptr)
    return nullptr;

  switch (u->op())
  {
  case TO_PAREN: return evaled;
  case TO_NOT: return evalNot(u, evaled->asLiteral());
  case TO_BNOT: return evalBNot(u, evaled->asLiteral());
  case TO_NEG: return evalNeg(u, evaled->asLiteral());
  case TO_TYPEOF: return evalTypeof(u, evaled->asLiteral());
  default: return nullptr;
  }
}

Expr *ExpressionEvaluator::evalTernary(TerExpr *ter) {
  Expr *a = evaluate(ter->a());
  Expr *b = evaluate(ter->b());
  Expr *c = evaluate(ter->c());

  if (!a || !b || !c)
    return nullptr;

  LiteralExpr *evaledA = a->asLiteral();
  LiteralExpr *evaledB = b->asLiteral();
  LiteralExpr *evaledC = c->asLiteral();

  switch (evaledA->kind())
  {
  case LK_STRING: return copyCoordinates(ter, b);
  case LK_FLOAT: return copyCoordinates(ter, evaledA->f() ? evaledB : evaledC);
  case LK_INT: return copyCoordinates(ter, evaledA->i() ? evaledB : evaledC);
  case LK_BOOL: return copyCoordinates(ter, evaledA->b() ? evaledB : evaledC);
  case LK_NULL: return copyCoordinates(ter, evaledC);
  default: return nullptr;
  }
}

Expr *ExpressionEvaluator::evalId(Id *id) {
  if (!_idResolver) {
    return nullptr;
  }

  Decl *d = _idResolver->resolveId(id);
  if (!d)
    return nullptr;

  switch (d->op()) {
  case TO_VAR: {
    VarDecl *v = static_cast<VarDecl *>(d);
    if (v->isAssignable())
      return nullptr;
    if (v->isDestructured())
      return nullptr;

    if (v->initializer())
      return evaluate(v->initializer());

    return nullptr;
  }
  case TO_CONST: return static_cast<ConstDecl *>(d)->value();
  default: return nullptr;
  }

}

Expr *ExpressionEvaluator::evalGetField(GetFieldExpr *gf) {
  Expr *receiver = gf->receiver();

  if (receiver->op() == TO_ID && _idResolver) {
    Decl *d = _idResolver->resolveId(receiver->asId());
    if (d && d->op() == TO_ENUM) {
      EnumDecl *enm = static_cast<EnumDecl *>(d);
      for (auto &e : enm->consts()) {
        if (strcmp(e.id, gf->fieldName()) == 0) {
          return e.val;
        }
      }
    }
  }

  return nullptr;
}

Expr *ExpressionEvaluator::evalGetIndex(GetTableExpr *gt) {
  Expr *receiver = gt->receiver();
  Expr *key = evaluate(gt->key());

  if (!key)
    return nullptr;

  LiteralExpr *lkey = key->asLiteral();

  if (receiver->op() == TO_ARRAYEXPR) {
    ArrayExpr *arr = static_cast<ArrayExpr *>(receiver);

    if (lkey->kind() != LK_INT)
      return nullptr;

    SQInteger idx = lkey->i();

    auto &values = arr->initialziers();

    if (idx < 0 || values.size() <= idx)
      return nullptr;

    return evaluate(values[idx]);
  }

  return nullptr;
}

Expr *ExpressionEvaluator::evalCall(CallExpr *call) {
  return nullptr;
}

Expr *ExpressionEvaluator::evaluate(Expr *expr) {
  switch (expr->op())
  {
  case TO_ID:
    return evalId(expr->asId());
  case TO_NULLC:
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
  case TO_USHR:
  case TO_SHR:
  case TO_SHL:
  case TO_MUL:
  case TO_DIV:
  case TO_MOD:
  case TO_ADD:
  case TO_SUB:
    return evalBinArith(static_cast<BinExpr *>(expr));
  case TO_PAREN:
  case TO_NOT:
  case TO_BNOT:
  case TO_NEG:
  case TO_TYPEOF:
    return evalUnary(static_cast<UnExpr *>(expr));
  case TO_LITERAL:
    return expr;
  case TO_GETFIELD:
    return evalGetField(expr->asGetField());
  case TO_GETTABLE:
    return evalGetIndex(expr->asGetTable());
  case TO_CALL:
    return evalCall(expr->asCallExpr());
  case TO_TERNARY:
    return evalTernary(static_cast<TerExpr *>(expr));
  default:
    return nullptr;
  }
}

Decl *ConstantFoldingOpt::resolveId(const Id *id) {
  return scope->findSymbol(id->id());
}

Node *ConstantFoldingOpt::transformValueDecl(ValueDecl *decl) {
  scope->declareSymbol(decl->name(), decl);
  return Transformer::transformValueDecl(decl);
}

Node *ConstantFoldingOpt::transformClassDecl(ClassDecl *klass) {
  if (klass->classKey() && klass->classKey()->op() == TO_ID) {
    scope->declareSymbol(klass->classKey()->asId()->id(), klass);
  }
  return Transformer::transformClassDecl(klass);
}

Node *ConstantFoldingOpt::transformFunctionDecl(FunctionDecl *f) {
  Scope functionScope(&arena, this);

  if (f->name()) {
    functionScope.declareSymbol(f->name(), f);
  }

  return Transformer::transformFunctionDecl(f);
}

Node *ConstantFoldingOpt::transformConstDecl(ConstDecl *cnst) {
  scope->declareSymbol(cnst->name(), cnst);
  return Transformer::transformConstDecl(cnst);
}

Node *ConstantFoldingOpt::transformEnumDecl(EnumDecl *enm) {
  scope->declareSymbol(enm->name(), enm);
  return Transformer::transformEnumDecl(enm);
}

Node *ConstantFoldingOpt::transformBlock(Block *b) {
  Scope blockScope(&arena, this);
  return Transformer::transformBlock(b);
}

} // namespace SQCompilation

#endif // !NO_COMPILER
