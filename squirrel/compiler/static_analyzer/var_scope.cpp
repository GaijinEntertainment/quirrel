#include <assert.h>

#include "var_scope.h"
#include "value_ref.h"
#include "naming.h"
#include "checker_visitor.h"
#include "compiler/compilationcontext.h"

namespace SQCompilation
{


void VarScope::copyFrom(const VarScope *other) {
  VarScope *l = this;
  const VarScope *r = other;

  while (l) {
    assert(l->owner == r->owner && "Scope corruption");

    auto &thisSymbols = l->symbols;
    auto &otherSymbols = r->symbols;
    auto it = otherSymbols.begin();
    auto ie = otherSymbols.end();

    while (it != ie) {
      thisSymbols[it->first] = it->second;
      ++it;
    }

    l = l->parent;
    r = r->parent;
  }
}

void VarScope::intersectScopes(const VarScope *other) {
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
        if (it->second->info == f->second->info)
          f->second->intersectValue(it->second);
      }
      ++it;
    }

    l = l->parent;
    r = r->parent;
  }
  evalId = std::max(evalId, other->evalId) + 1; // -V522
}


void VarScope::mergeUnbalanced(const VarScope *other) {
  VarScope *lhs = this;
  const VarScope *rhs = other;

  while (lhs->depth > rhs->depth) {
    lhs = lhs->parent;
  }

  while (rhs->depth > lhs->depth) {
    rhs = rhs->parent;
  }

  lhs->merge(rhs);
}

void VarScope::merge(const VarScope *other) {
  VarScope *l = this;
  const VarScope *r = other;

  while (l) {
    assert(l->depth == r->depth && "Scope corruption");
    assert(l->owner == r->owner && "Scope corruption");

    auto &thisSymbols = l->symbols;
    auto &otherSymbols = r->symbols;
    auto it = otherSymbols.begin();
    auto ie = otherSymbols.end();
    auto te = thisSymbols.end();

    while (it != ie) {
      auto f = thisSymbols.find(it->first);
      if (f != te) {
        if (it->second->info == f->second->info) // lambdas declared on the same line could have same names
          f->second->merge(it->second);
      }
      else {
        it->second->kill(VRS_PARTIALLY);
        thisSymbols[it->first] = it->second;
      }
      ++it;
    }

    l = l->parent;
    r = r->parent;
  }

  evalId = std::max(evalId, other->evalId) + 1;
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
  void *mem = a->allocate(sizeof(VarScope));
  VarScope *thisCopy = new(mem) VarScope(owner, parentCopy);

  for (auto &kv : symbols) {
    const SQChar *k = kv.first;
    ValueRef *v = kv.second;
    void *mem = a->allocate(sizeof(ValueRef));
    ValueRef *vcopy = new(mem) ValueRef(v->info, v->evalIndex);

    if (!v->isConstant() && forClosure) {
      // if we analyze closure we cannot rely on existed assignable values
      vcopy->state = VRS_UNKNOWN;
      vcopy->expression = nullptr;
      vcopy->flagsNegative = vcopy->flagsPositive = 0;
      vcopy->assigned = v->assigned;
      vcopy->lastAssigneeScope = v->lastAssigneeScope;
    }
    else {
      memcpy(vcopy, v, sizeof(ValueRef));
      vcopy->assigned = false;
      vcopy->lastAssigneeScope = nullptr;
    }
    thisCopy->symbols[k] = vcopy;
  }

  return thisCopy;
}


void VarScope::checkUnusedSymbols(CheckerVisitor *checker) {

  for (auto &s : symbols) {
    const SQChar *n = s.first;
    const ValueRef *v = s.second;

    if (strcmp(n, "this") == 0)
      continue;

    SymbolInfo *info = v->info;

    if (info->kind == SK_ENUM_CONST)
      continue;

    if (!info->used && n[0] != '_') {
      if (info->kind == SK_IMPORT) {
        const ImportInfo *import = info->declarator.imp;
        checker->reportImportSlot(import->line, import->column, import->name);
      }
      else {
        checker->report(info->extractPointedNode(), DiagnosticsId::DI_DECLARED_NEVER_USED, info->contextName(), n);
        // TODO: add hint for param/exception name about silencing it with '_' prefix
      }
    }
    else if (info->used && n[0] == '_') {
      if (info->kind == SK_PARAM || info->kind == SK_FOREACH)
        checker->report(info->extractPointedNode(), DiagnosticsId::DI_INVALID_UNDERSCORE, info->contextName(), n);
    }
  }
}

}
