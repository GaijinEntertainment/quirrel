#pragma once

#include "compiler/ast.h"
#include "analyzer_internal.h"


namespace SQCompilation
{

struct ValueRef;
class CheckerVisitor;


struct VarScope
{
  VarScope(const FunctionDecl *o, VarScope *p = nullptr)
    : owner(o), parent(p), depth(p ? p->depth + 1 : 0), evalId(p ? p->evalId + 1 : 0)
  {}

  ~VarScope()
  {
    if (parent)
      parent->~VarScope();
    symbols.clear();
  }


  int32_t evalId;
  const int32_t depth;
  const FunctionDecl *owner;
  VarScope *parent;
  std::unordered_map<const SQChar *, ValueRef *, StringHasher, StringEqualer> symbols;

  void intersectScopes(const VarScope *other);

  void merge(const VarScope *other);
  VarScope *copy(Arena *a, bool forClosure = false) const;
  void copyFrom(const VarScope *other);

  void mergeUnbalanced(const VarScope *other);

  VarScope *findScope(const FunctionDecl *own);
  void checkUnusedSymbols(CheckerVisitor *v);
};

}
