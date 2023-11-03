#ifndef _SQ_OPT_EVALUATOR_
#define _SQ_OPT_EVALUATOR_ 1

#include "../sqast.h"
#include "../arena.h"
#include "../sqstate.h"
#include "../sqcompilationcontext.h"

#include <map>
#include <set>

namespace SQCompilation {

class IIdResolver {
public:
  virtual Decl *resolveId(const Id *id) = 0;
};

class ExpressionEvaluator {
  Arena *_astArena;
  IIdResolver *_idResolver;

  SQChar *copyString(const SQChar *s, int32_t len) {
    size_t memLen = (len + 1) * sizeof(SQChar);
    SQChar *buf = (SQChar *)_astArena->allocate(memLen);
    memcpy(buf, s, memLen);
    return buf;
  }

  template<typename N, typename ... Args>
  N *newNode(Args... args) {
    return new (_astArena) N(args...);
  }

  LiteralExpr *evalNullCExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalOrOrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalAndAndExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalOrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalXorOrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalAndExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalNeExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalEqExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *eval3CmpExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalGeExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalGtExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalLeExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalLtExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalUshrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalShrExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalShlExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalMulExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalDivExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalModExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalAddExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);
  LiteralExpr *evalSubExpr(const BinExpr *orig, const LiteralExpr *l, const LiteralExpr *r);

  LiteralExpr *evalNot(const UnExpr *orig, const LiteralExpr *arg);
  LiteralExpr *evalBNot(const UnExpr *orig, const LiteralExpr *arg);
  LiteralExpr *evalNeg(const UnExpr *orig, const LiteralExpr *arg);
  LiteralExpr *evalTypeof(const UnExpr *orig, const LiteralExpr *arg);

  Expr *evalId(Id *id);
  Expr *evalBinArith(BinExpr *bin);
  Expr *evalUnary(UnExpr *expr);
  Expr *evalGetField(GetFieldExpr *gf);
  Expr *evalGetIndex(GetTableExpr *gf);
  Expr *evalCall(CallExpr *call);
  Expr *evalTernary(TerExpr *expr);

public:
  ExpressionEvaluator(Arena *arena, IIdResolver *idResolver) : _astArena(arena), _idResolver(idResolver) {}

  Expr *evaluate(Expr *);
};

class ConstantFoldingOpt : public Transformer, public IIdResolver {
  ExpressionEvaluator evaluator;
  SQSharedState *ss;

  struct Scope {

    struct cmp_str
    {
      bool operator()(char const *a, char const *b) const
      {
        return std::strcmp(a, b) < 0;
      }
    };
    typedef ArenaMap<const char *, Decl *, cmp_str> SymbolsMap;

    Scope(Arena *arena, ConstantFoldingOpt *opt) : symbols(SymbolsMap::Allocator(arena)), v(opt) {
      parent = opt->scope;
      opt->scope = this;
    }

    ~Scope() {
      v->scope = parent;
    }


    void declareSymbol(const SQChar *name, Decl *d) {
      symbols[name] = d;
    }

    Decl *findSymbol(const SQChar *name) {
      auto it = symbols.find(name);

      if (it != symbols.end())
        return it->second;

      if (parent) {
        return parent->findSymbol(name);
      }

      return nullptr;
    }

    struct Scope *parent;
    ConstantFoldingOpt *v;
    SymbolsMap symbols;
  };

  Arena arena;

  struct Scope *scope;
  struct Scope rootScope;

public:
  ConstantFoldingOpt(SQSharedState *s, Arena *astArena)
    : ss(s)
    , evaluator(astArena, this)
    , arena(s->_alloc_ctx, "ConstantFoldingOpt")
    , scope(nullptr)
    , rootScope(&arena, this) {
  }

  Node *transformExpr(Expr *expr) {
    Expr *transformed = Transformer::transformExpr(expr)->asExpression();
    Expr *evaluated = evaluator.evaluate(transformed);

    if (evaluated)
      return evaluated;

    return transformed;
  }

  Node *transformValueDecl(ValueDecl *decl);
  Node *transformClassDecl(ClassDecl *cls);
  Node *transformFunctionDecl(FunctionDecl *f);
  Node *transformConstDecl(ConstDecl *cnst);
  Node *transformEnumDecl(EnumDecl *enm);

  Node *transformBlock(Block *b);

  Decl *resolveId(const Id *id);

  Node *fold(Node *root) {
    return root->transform(this);
  }

  void run(RootBlock *r) {
    if (!ss->checkCompilationOption(CompilationOptions::CO_CONSTANT_FOLDING_OPT))
      return;

    Node *f = fold(r);
    assert(f == r);
  }
};

} // namespace SQCompilation

#endif // !_SQ_OPT_EVALUATOR_