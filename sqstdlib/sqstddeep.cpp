/* see copyright notice in squirrel.h */
#include <squirrel.h>
#include <assert.h>
#include <new>
#include <squirrel/sqvm.h>
#include <squirrel/sqstate.h>
#include <squirrel/sqtable.h>
#include <squirrel/sqclass.h>
#include <squirrel/sqarray.h>
#include <squirrel/sqfuncproto.h>
#include <squirrel/sqclosure.h>
#include <sqstddeep.h>

// script isEqual overflows at this depth
#define SQ_DEEP_EQUAL_MAX_DEPTH 10997
// the VM frees nested containers recursively
#define SQ_DEEP_SHARED_CLONE_MAX_DEPTH 1000

namespace
{

enum Step
{
  STEP_DONE,
  STEP_PUSHED,
  STEP_ERROR
};

template <typename T, unsigned N = 16>
class FrameStack
{
public:
  explicit FrameStack(SQAllocContext ctx) : heap(ctx) {} //-V730
  ~FrameStack()
  {
    while (count)
      pop();
  }
  unsigned size() const { return count; }
  T &top() { return count <= N ? local()[count - 1] : heap.top(); }
  void push(T &&f)
  {
    if (count < N)
      new (&local()[count]) T(std::move(f));
    else
      heap.push_back(std::move(f));
    count++;
  }
  void pop()
  {
    count--;
    if (count < N)
      local()[count].~T();
    else
      heap.pop_back();
  }

private:
  T *local() { return reinterpret_cast<T *>(buf); }
  alignas(T) unsigned char buf[sizeof(T) * N];
  sqvector<T> heap;
  unsigned count = 0;
};

template <typename T>
Step done(SQObjectPtr &res, const T &val)
{
  res = val;
  return STEP_DONE;
}

void mark_frozen_read(const SQObject &container, SQObjectPtr &val) { val._flags |= sq_objflags(container) & SQOBJ_FLAG_IMMUTABLE; }

bool call_fn(HSQUIRRELVM v, const SQObjectPtr &fn, const SQObjectPtr &self, const SQObjectPtr *args, int nargs, SQObjectPtr &res)
{
  v->Push(self);
  for (int i = 0; i < nargs; i++)
    v->Push(args[i]);
  bool ok = v->Call(fn, nargs + 1, v->_top - (nargs + 1), res, SQFalse);
  v->Pop(nargs + 1);
  return ok;
}

bool call_method(HSQUIRRELVM v, const SQObjectPtr &obj, const SQObjectPtr &name, const SQObjectPtr *args, int nargs,
  SQObjectPtr &res)
{
  SQObjectPtr fn;
  return v->Get(obj, name, fn, 0) && call_fn(v, fn, obj, args, nargs, res);
}

// the lookup flags of the `in` operator
bool exists(HSQUIRRELVM v, const SQObjectPtr &obj, const SQObjectPtr &key)
{
  SQObjectPtr tmp;
  return v->Get(obj, key, tmp, GET_FLAG_DO_NOT_RAISE_ERROR | GET_FLAG_NO_TYPE_METHODS);
}

bool plain_table(const SQObjectPtr &o, const SQObjectPtr &shadowed_method)
{
  return sq_istable(o) && !_table(o)->_Get(shadowed_method);
}

SQInteger plain_len(const SQObjectPtr &o) { return sq_isarray(o) ? _array(o)->Size() : _table(o)->CountUsed(); }

bool same_value(bool strict, const SQObject &a, const SQObject &b)
{
  if (!strict)
    return SQVM::IsEqual(a, b);
  return sq_type(a) == sq_type(b) && (sq_isfloat(a) ? !memcmp(&_float(a), &_float(b), sizeof(SQFloat)) : _rawval(a) == _rawval(b));
}

struct EqFrame
{
  SQObjectPtr a, b, pos;
  bool fast;
};

struct EqCtx
{
  HSQUIRRELVM v;
  SQObjectPtr self, comparers, isEqualStr, lenStr;
  bool useComparers = false, strict = false;
  FrameStack<EqFrame> stack;
  explicit EqCtx(HSQUIRRELVM vm) : v(vm), stack(_ss(vm)->_alloc_ctx) {}
};

Step eq_instance(EqCtx &c, const SQObjectPtr &a, const SQObjectPtr &b, SQObjectPtr &res)
{
  HSQUIRRELVM v = c.v;
  if (exists(v, a, c.isEqualStr))
    return call_method(v, a, c.isEqualStr, &b, 1, res) ? STEP_DONE : STEP_ERROR;
  if (c.useComparers)
  {
    if (!sq_istable(c.comparers))
    {
      v->Raise_Error("cannot iterate %s", GetTypeName(c.comparers));
      return STEP_ERROR;
    }
    SQObjectPtr pos, key, fn, r;
    for (SQInteger next; (next = _table(c.comparers)->Next(false, pos, key, fn)) >= 0;)
    {
      pos = next;
      SQObjectType kt = sq_type(key);
      if (kt == OT_CLOSURE || kt == OT_NATIVECLOSURE)
      {
        if (!call_fn(v, key, c.self, &a, 1, r) || (!SQVM::IsFalse(r) && !call_fn(v, key, c.self, &b, 1, r)))
          return STEP_ERROR;
        if (SQVM::IsFalse(r))
          continue;
      }
      else if (kt != OT_CLASS || !SQVM::IsInstanceOf(a, _class(key)) || !SQVM::IsInstanceOf(b, _class(key)))
        continue;
      SQObjectPtr args[2] = {a, b};
      return call_fn(v, fn, c.self, args, 2, res) ? STEP_DONE : STEP_ERROR;
    }
  }
  return done(res, false);
}

Step eq_pair(EqCtx &c, const SQObjectPtr &a, const SQObjectPtr &b, SQObjectPtr &res)
{
  HSQUIRRELVM v = c.v;
  if (same_value(c.strict, a, b))
    return done(res, true);
  SQObjectType t = sq_type(a);
  const SQObjectPtr &typeName = _ss(v)->TypeName(t);
  if (_string(typeName) != _string(_ss(v)->TypeName(sq_type(b))))
    return done(res, false);
  if (c.useComparers && exists(v, c.comparers, typeName))
  {
    SQObjectPtr args[2] = {a, b};
    return call_method(v, c.comparers, typeName, args, 2, res) ? STEP_DONE : STEP_ERROR;
  }
  if (t == OT_INSTANCE)
    return eq_instance(c, a, b, res);
  if (t != OT_TABLE && t != OT_ARRAY && t != OT_CLASS)
    return done(res, false);
  bool fast = sq_isarray(a) || (plain_table(a, c.lenStr) && plain_table(b, c.lenStr));
  if (!fast)
  {
    SQObjectPtr na, nb;
    if (!call_method(v, a, c.lenStr, nullptr, 0, na) || !call_method(v, b, c.lenStr, nullptr, 0, nb))
      return STEP_ERROR;
    if (!SQVM::IsEqual(na, nb))
      return done(res, false);
  }
  else if (plain_len(a) != plain_len(b))
    return done(res, false);
  if (c.stack.size() >= SQ_DEEP_EQUAL_MAX_DEPTH)
  {
    v->Raise_Error("stack overflow, cannot resize stack");
    return STEP_ERROR;
  }
  c.stack.push({a, b, {}, fast});
  return STEP_PUSHED;
}

template <bool strict>
SQInteger deep_equal(HSQUIRRELVM v)
{
  const SQObjectPtr &arg1 = stack_get(v, 2), &arg2 = stack_get(v, 3);
  bool same = same_value(strict, arg1, arg2);
  if (same || _string(_ss(v)->TypeName(sq_type(arg1))) != _string(_ss(v)->TypeName(sq_type(arg2))))
  {
    sq_pushbool(v, same);
    return 1;
  }
  SQInteger top = sq_gettop(v);
  EqCtx c(v);
  c.strict = strict;
  c.self = stack_get(v, 1);
  c.isEqualStr = stack_get(v, top - 1);
  c.lenStr = stack_get(v, top);
  if (top > 5)
  {
    c.comparers = stack_get(v, 4);
    c.useComparers = true;
  }
  SQObjectPtr a = arg1, b = arg2, res;
  Step root = eq_pair(c, a, b, res);
  if (root == STEP_ERROR)
    return SQ_ERROR;
  while (c.stack.size())
  {
    EqFrame &f = c.stack.top();
    SQObjectPtr key, va, vb;
    SQInteger next;
    if (sq_istable(f.a))
      next = _table(f.a)->Next(false, f.pos, key, va);
    else if (sq_isarray(f.a))
      next = _array(f.a)->Next(f.pos, key, va);
    else
      next = _class(f.a)->Next(f.pos, key, va);
    if (next < 0)
    {
      c.stack.pop();
      continue;
    }
    f.pos = next;
    mark_frozen_read(f.a, key);
    mark_frozen_read(f.a, va);
    bool found;
    if (f.fast)
    {
      found = sq_istable(f.b) ? _table(f.b)->Get(key, vb) : _array(f.b)->Get(_integer(key), vb);
      mark_frozen_read(f.b, vb);
    }
    else
      found = v->Get(f.b, key, vb, GET_FLAG_DO_NOT_RAISE_ERROR | GET_FLAG_NO_TYPE_METHODS);
    if (!found)
      break;
    Step s = eq_pair(c, va, vb, res);
    if (s == STEP_ERROR)
      return SQ_ERROR;
    if (s == STEP_DONE && SQVM::IsFalse(res))
      break;
  }
  if (root == STEP_PUSHED)
    res = !c.stack.size();
  v->Push(res);
  return 1;
}

struct ClFrame
{
  SQObjectPtr src, dst, pos, slot;
};

struct ClCtx
{
  HSQUIRRELVM v;
  SQInteger depthLimit = 0;
  SQObjectPtr mapStr, deepcloneStr, memo, cloner;
  FrameStack<ClFrame> stack;
  explicit ClCtx(HSQUIRRELVM vm) : v(vm), stack(_ss(vm)->_alloc_ctx) {}
  bool shared() const { return !sq_isnull(memo); }
};

SQInteger deep_clone(HSQUIRRELVM v);

// map() passes one argument at nparamscheck 2
const SQObjectPtr &get_cloner(ClCtx &c)
{
  if (sq_isnull(c.cloner))
  {
    SQNativeClosure *nc = SQNativeClosure::Create(_ss(c.v), deep_clone, 3);
    nc->_nparamscheck = 2;
    nc->_outervalues[0] = c.mapStr;
    nc->_outervalues[1] = c.deepcloneStr;
    nc->_outervalues[2] = c.memo;
    c.cloner = nc;
  }
  return c.cloner;
}

void cl_store(const SQObjectPtr &dst, const SQObjectPtr &slot, const SQObjectPtr &val)
{
  if (sq_istable(dst))
    _table(dst)->NewSlot(slot, val);
  else
    _array(dst)->Append(val);
}

Step cl_value(ClCtx &c, const SQObjectPtr &val, SQObjectPtr &out)
{
  HSQUIRRELVM v = c.v;
  SQObjectType t = sq_type(val);
  // script deep_clone takes a native call level for each value
  if (!c.shared() && SQInteger(c.stack.size()) >= c.depthLimit)
  {
    v->Raise_Error("Native stack overflow");
    return STEP_ERROR;
  }
  if (c.shared() && (t == OT_TABLE || t == OT_ARRAY || t == OT_INSTANCE) && _table(c.memo)->Get(val, out))
    return STEP_DONE;
  SQObjectPtr dst;
  if (t == OT_ARRAY)
  {
    dst = SQArray::Create(_ss(v), 0);
    _array(dst)->Reserve(_array(val)->Size());
  }
  else if (t == OT_TABLE && (c.shared() || plain_table(val, c.mapStr)))
    dst = SQTable::Create(_ss(v), _table(val)->CountUsed());
  else if ((t == OT_TABLE || t == OT_CLASS) && !c.shared())
    return call_method(v, val, c.mapStr, &get_cloner(c), 1, out) ? STEP_DONE : STEP_ERROR;
  else if (t == OT_INSTANCE && c.shared() && exists(v, val, c.deepcloneStr))
  {
    if (!call_method(v, val, c.deepcloneStr, &get_cloner(c), 1, out))
      return STEP_ERROR;
    _table(c.memo)->NewSlot(val, out);
    return STEP_DONE;
  }
  else
    return done(out, val);
  if (c.shared() && SQInteger(c.stack.size()) >= c.depthLimit)
  {
    v->Raise_Error("copy_graph: the value is nested deeper than %d levels", int(c.depthLimit));
    return STEP_ERROR;
  }
  if (c.shared())
    _table(c.memo)->NewSlot(val, dst);
  c.stack.push({val, dst});
  return STEP_PUSHED;
}

SQInteger next_slot(const SQObjectPtr &container, bool weakrefs, const SQObjectPtr &pos, SQObjectPtr &key, SQObjectPtr &val)
{
  if (sq_istable(container))
    return _table(container)->Next(weakrefs, pos, key, val);
  SQInteger next = _array(container)->Next(pos, key, val);
  if (weakrefs && next >= 0)
    val = _array(container)->_values[_integer(key)];
  return next;
}

SQInteger deep_clone(HSQUIRRELVM v)
{
  SQObjectPtr root = stack_get(v, 2);
  SQObjectType t = sq_type(root);
  if (t != OT_TABLE && t != OT_ARRAY && t != OT_CLASS && t != OT_INSTANCE)
  {
    v->Push(root);
    return 1;
  }
  SQInteger top = sq_gettop(v);
  ClCtx c(v);
  c.mapStr = stack_get(v, top - 2);
  c.deepcloneStr = stack_get(v, top - 1);
  c.memo = stack_get(v, top);
  if (sq_isbool(c.memo))
    c.memo = SQTable::Create(_ss(v), 0);
  // two native calls per script deep_clone level
  c.depthLimit = c.shared() ? SQ_DEEP_SHARED_CLONE_MAX_DEPTH : (MAX_NATIVE_CALLS + 2 - v->_nnativecalls) / 2;
  SQObjectPtr out;
  Step s = cl_value(c, root, out);
  if (s == STEP_ERROR)
    return SQ_ERROR;
  while (c.stack.size())
  {
    ClFrame &f = c.stack.top();
    SQObjectPtr slot, val;
    SQInteger next = next_slot(f.src, c.shared(), f.pos, slot, val);
    if (next < 0)
    {
      if (sq_isarray(f.dst))
        _array(f.dst)->ShrinkIfNeeded();
      out = std::move(f.dst);
      slot = std::move(f.slot);
      c.stack.pop();
      if (c.stack.size())
        cl_store(c.stack.top().dst, slot, out);
      continue;
    }
    f.pos = next;
    s = cl_value(c, val, out);
    // script map() skips null-throwing callbacks
    if (s == STEP_ERROR && (c.shared() || !sq_isnull(v->_lasterror)))
      return SQ_ERROR;
    if (s == STEP_DONE)
      cl_store(f.dst, slot, out);
    else if (s == STEP_PUSHED)
      c.stack.top().slot = slot;
  }
  v->Push(out);
  return 1;
}

} // namespace

SQRESULT sqstd_register_deeplib(HSQUIRRELVM v)
{
  sq_pushstring(v, "len", -1);
  sq_pushstring(v, "isEqual", -1);
  sq_new_closure_slot_from_decl_string(v, deep_equal<false>, 2, "is_relaxed_equal(a, b, [comparers: table|null]): any",
    SQ_DOC("Compares two values deeply: equal tables and arrays, and instances by their isEqual() method"));
  sq_pushstring(v, "len", -1);
  sq_pushstring(v, "isEqual", -1);
  sq_new_closure_slot_from_decl_string(v, deep_equal<true>, 2, "is_strict_equal(a, b, [comparers: table|null]): any",
    SQ_DOC("Compares as is_relaxed_equal() does, but numbers are equal only with the same type and the same bits"));
  sq_pushnull(v);
  sq_pushstring(v, "_deepclone", -1);
  sq_pushstring(v, "map", -1);
  sq_new_closure_slot_from_decl_string(v, deep_clone, 3, "copy(value): any",
    SQ_DOC("Copies tables and arrays deeply, as deep_clone() of underscore.nut does"));
  sq_pushbool(v, SQTrue);
  sq_pushstring(v, "_deepclone", -1);
  sq_pushstring(v, "map", -1);
  sq_new_closure_slot_from_decl_string(v, deep_clone, 3, "copy_graph(value): any",
    SQ_DOC("Copies the object graph: shared parts stay shared, and an instance is copied by its _deepclone()"));
  return SQ_OK;
}
