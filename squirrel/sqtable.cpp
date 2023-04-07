/*
see copyright notice in squirrel.h
*/
#include "sqpcheader.h"
#include "sqvm.h"
#include "sqtable.h"
#include "sqfuncproto.h"
#include "sqclosure.h"

#define _FAST_CLONE

SQTable::SQTable(SQSharedState *ss,SQInteger nInitialSize) :
    _alloc_ctx(ss->_alloc_ctx)
{
    SQInteger pow2size=MINPOWER2;
    while(nInitialSize>pow2size)pow2size=pow2size<<1;
    AllocNodes(pow2size);
    _usednodes = 0;
    _delegate = NULL;
    INIT_CHAIN();
    ADD_TO_CHAIN(&_sharedstate->_gc_chain,this);
}

void SQTable::Remove(const SQObjectPtr &key)
{
    _HashNode *n = _Get(key, HashObj(key) & _numofnodes_minus_one);
    if (n) {
        n->val.Null();
        n->key.Null();
        VT_CLEAR_SINGLE(n);
        _usednodes--;
        Rehash(false);
    }
}

void SQTable::AllocNodes(SQInteger nSize)
{
    _HashNode *nodes=(_HashNode *)SQ_MALLOC(_alloc_ctx, sizeof(_HashNode)*nSize);
    for(SQInteger i=0;i<nSize;i++){
        _HashNode &n = nodes[i];
        new (&n) _HashNode;
        n.next=NULL;
    }
    _numofnodes_minus_one=(uint32_t)(nSize-1);
    _nodes=nodes;
    _firstfree=&_nodes[_numofnodes_minus_one];
}

void SQTable::Rehash(bool force)
{
    SQInteger oldsize=_numofnodes_minus_one+1;
    //prevent problems with the integer division
    if(oldsize<4)oldsize=4;
    _HashNode *nold=_nodes;
    SQInteger nelems=CountUsed();
    if (nelems >= oldsize-oldsize/4)  /* using more than 3/4? */
        AllocNodes(oldsize*2);
    else if (nelems <= oldsize/4 &&  /* less than 1/4? */
        oldsize > MINPOWER2)
        AllocNodes(oldsize/2);
    else if(force)
        AllocNodes(oldsize);
    else
        return;
    _usednodes = 0;
    for (SQInteger i=0; i<oldsize; i++) {
        _HashNode *old = nold+i;
        if (sq_type(old->key) != OT_NULL)
            NewSlot(old->key,old->val  VT_REF(old));
    }
    for(SQInteger k=0;k<oldsize;k++)
        nold[k].~_HashNode();
    SQ_FREE(_alloc_ctx, nold, oldsize*sizeof(_HashNode));
}

SQTable *SQTable::Clone()
{
    SQTable *nt=Create(_opt_ss(this),_numofnodes_minus_one+1);
#ifdef _FAST_CLONE
    _HashNode *basesrc = _nodes;
    _HashNode *basedst = nt->_nodes;
    _HashNode *src = _nodes;
    _HashNode *dst = nt->_nodes;
    for(uint32_t n = 0; n <= _numofnodes_minus_one; n++) {
        dst->key = src->key;
        dst->val = src->val;
        VT_COPY_SINGLE(src, dst);
        VT_TRACE_SINGLE(dst, dst->val, _ss(this)->_root_vm);
        if(src->next) {
            assert(src->next > basesrc);
            dst->next = basedst + (src->next - basesrc);
            assert(dst != dst->next);
        }
        dst++;
        src++;
    }
    assert(_firstfree >= basesrc);
    assert(_firstfree != NULL);
    nt->_firstfree = basedst + (_firstfree - basesrc);
    nt->_usednodes = _usednodes;
#else
    SQInteger ridx=0;
    SQObjectPtr key,val;
    while((ridx=Next(true,ridx,key,val))!=-1){
        nt->NewSlot(key,val VT_CODE(VT_COMMA &_nodes[ridx].varTrace));
    }
#endif
    nt->SetDelegate(_delegate);
    return nt;
}

SQTable::_HashNode *SQTable::_Get(const SQObjectPtr &key) const
{
    if(sq_type(key) == OT_NULL)
        return nullptr;

    if (sq_type(key) == OT_STRING)
        return _GetStr(_rawval(key), _string(key)->_hash & _numofnodes_minus_one);
    else
        return _Get(key, HashObj(key) & _numofnodes_minus_one);
}

bool SQTable::Get(const SQObjectPtr &key,SQObjectPtr &val) const
{
    const _HashNode *n = _Get(key);
    if (n) {
        val = _realval(n->val);
        return true;
    }
    return false;
}

bool SQTable::GetStrToInt(const SQObjectPtr &key,uint32_t &val) const//for class members
{
    assert(sq_type(key) == OT_STRING);
    const _HashNode *n = _GetStr(_rawval(key), _string(key)->_hash & _numofnodes_minus_one);
    if (!n)
      return false;
    assert(sq_type(n->val) == OT_INTEGER);
    val = _integer(n->val);
    return true;
}

#if SQ_VAR_TRACE_ENABLED == 1
VarTrace * SQTable::GetVarTracePtr(const SQObjectPtr &key)
{
  _HashNode *n = _Get(key, HashObj(key) & _numofnodes_minus_one);
  if (n)
    return &(n->varTrace);
  else
    return NULL;
}
#endif


bool SQTable::NewSlot(const SQObjectPtr &key,const SQObjectPtr &val  VT_DECL_ARG)
{
    assert(sq_type(key) != OT_NULL);
    SQHash h = HashObj(key) & _numofnodes_minus_one;
    _HashNode *n = _Get(key, h);
    if (n) {
        n->val = val;
        VT_CODE(if (var_trace_arg) n->varTrace = *var_trace_arg);
        VT_TRACE_SINGLE(n, val, _ss(this)->_root_vm);
        return false;
    }
    _HashNode *mp = &_nodes[h];
    n = mp;

    //key not found I'll insert it
    //main pos is not free

    if(sq_type(mp->key) != OT_NULL) {
        n = _firstfree;  /* get a free place */
        SQHash mph = HashObj(mp->key) & _numofnodes_minus_one;
        _HashNode *othern;  /* main position of colliding node */

        if (mp > n && (othern = &_nodes[mph]) != mp){
            /* yes; move colliding node into free position */
            while (othern->next != mp){
                assert(othern->next != NULL);
                othern = othern->next;  /* find previous */
            }
            othern->next = n;  /* redo the chain with `n' in place of `mp' */
            n->key = mp->key;
            n->val = mp->val;/* copy colliding node into free pos. (mp->next also goes) */
            n->next = mp->next;
            VT_COPY_SINGLE(mp, n);
            mp->key.Null();
            mp->val.Null();
            VT_CLEAR_SINGLE(mp);
            mp->next = NULL;  /* now `mp' is free */
        }
        else{
            /* new node will go into free position */
            n->next = mp->next;  /* chain new position */
            mp->next = n;
            mp = n;
        }
    }
    mp->key = key;

    for (;;) {  /* correct `firstfree' */
        if (sq_type(_firstfree->key) == OT_NULL && _firstfree->next == NULL) {
            mp->val = val;
            VT_CODE(if (var_trace_arg) mp->varTrace = *var_trace_arg);
            VT_TRACE_SINGLE(mp, val, _ss(this)->_root_vm);
            _usednodes++;
            return true;  /* OK; table still has a free place */
        }
        else if (_firstfree == _nodes) break;  /* cannot decrement from here */
        else (_firstfree)--;
    }
    Rehash(true);
    return NewSlot(key, val  VT_CODE(VT_COMMA var_trace_arg));
}

SQInteger SQTable::Next(bool getweakrefs,const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval)
{
    uint32_t idx = (uint32_t)TranslateIndex(refpos);
    while (idx <= _numofnodes_minus_one) {
        if(sq_type(_nodes[idx].key) != OT_NULL) {
            //first found
            _HashNode &n = _nodes[idx];
            outkey = n.key;
            outval = getweakrefs?(SQObject)n.val:_realval(n.val);
            //return idx for the next iteration
            return ++idx;
        }
        ++idx;
    }
    //nothing to iterate anymore
    return -1;
}


bool SQTable::Set(const SQObjectPtr &key, const SQObjectPtr &val)
{
    _HashNode *n = _Get(key);
    if (n) {
        n->val = val;
        VT_TRACE_SINGLE(n, val, _ss(this)->_root_vm);
        return true;
    }
    return false;
}

void SQTable::_ClearNodes()
{
    for (uint32_t i = 0; i <= _numofnodes_minus_one; i++) {
      _HashNode &n = _nodes[i];
      n.key.Null();
      n.val.Null();
      VT_CLEAR_SINGLE((&n));
    }
}

void SQTable::Finalize()
{
    _ClearNodes();
    SetDelegate(NULL);
}

void SQTable::Clear()
{
    _ClearNodes();
    _usednodes = 0;
    Rehash(true);
}
