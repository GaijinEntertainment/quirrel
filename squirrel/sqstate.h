/*  see copyright notice in squirrel.h */
#ifndef _SQSTATE_H_
#define _SQSTATE_H_

#include "squtils.h"
#include "sqobject.h"
struct SQString;
struct SQTable;
//max number of character for a printed number
#define NUMBER_MAX_CHAR 50

struct SQStringTable
{
    SQStringTable(SQSharedState*ss);
    ~SQStringTable();
    SQString *Add(const SQChar *,SQInteger len);
    void Remove(SQString *);
private:
    void Resize(SQInteger size);
    void AllocNodes(SQInteger size);
    SQString **_strings;
    SQUnsignedInteger _numofslots;
    SQUnsignedInteger _slotused;
    SQSharedState *_sharedstate;
};

struct RefTable {
    struct RefNode {
        SQObjectPtr obj;
        SQUnsignedInteger refs;
        struct RefNode *next;
    };
    RefTable(SQAllocContext ctx);
    ~RefTable();
    void AddRef(SQObject &obj);
    SQBool Release(SQObject &obj);
    SQUnsignedInteger GetRefCount(SQObject &obj);
#ifndef NO_GARBAGE_COLLECTOR
    void Mark(SQCollectable **chain);
#endif
    void Finalize();
private:
    RefNode *Get(SQObject &obj,SQHash &mainpos,RefNode **prev,bool add);
    RefNode *Add(SQHash mainpos,SQObject &obj);
    void Resize(SQUnsignedInteger size);
    void AllocNodes(SQUnsignedInteger size);
    SQUnsignedInteger _numofslots;
    SQUnsignedInteger _slotused;
    RefNode *_nodes;
    RefNode *_freelist;
    RefNode **_buckets;
    SQAllocContext _alloc_ctx;
};

#define ADD_STRING(ss,str,len) ss->_stringtable->Add(str,len)
#define REMOVE_STRING(ss,bstr) ss->_stringtable->Remove(bstr)

struct SQObjectPtr;

struct SQSharedState
{
    SQSharedState(SQAllocContext allocctx);
    ~SQSharedState();
    void Init();
public:
    bool checkCompilationOption(SQUnsignedInteger co) const {
        return (compilationOptions & co) != 0;
    }

    void enableCompilationOption(SQUnsignedInteger co) {
        compilationOptions |= co;
    }

    void disableCompilationOption(SQUnsignedInteger co) {
        compilationOptions &= ~co;
    }
    SQChar* GetScratchPad(SQInteger size);
    SQInteger GetMetaMethodIdxByName(const SQObjectPtr &name);
#ifndef NO_GARBAGE_COLLECTOR
    SQInteger CollectGarbage(SQVM *vm);
    void RunMark(SQVM *vm,SQCollectable **tchain);
    SQInteger ResurrectUnreachable(SQVM *vm);
    static void MarkObject(SQObjectPtr &o,SQCollectable **chain);
#endif
    SQAllocContext _alloc_ctx;
    SQObjectPtrVec *_metamethods;
    SQObjectPtr _metamethodsmap;
    SQObjectPtrVec *_systemstrings;
    SQObjectPtrVec *_types;
    SQStringTable *_stringtable;
    RefTable _refs_table;
    SQObjectPtr _registry;
    SQObjectPtr _consts;
    SQObjectPtr _constructoridx;
#ifndef NO_GARBAGE_COLLECTOR
    SQCollectable *_gc_chain;
#endif
    SQObjectPtr _root_vm;
    SQObjectPtr _table_default_delegate;
    static const SQRegFunction _table_default_delegate_funcz[];
    SQObjectPtr _array_default_delegate;
    static const SQRegFunction _array_default_delegate_funcz[];
    SQObjectPtr _string_default_delegate;
    static const SQRegFunction _string_default_delegate_funcz[];
    SQObjectPtr _number_default_delegate;
    static const SQRegFunction _number_default_delegate_funcz[];
    SQObjectPtr _generator_default_delegate;
    static const SQRegFunction _generator_default_delegate_funcz[];
    SQObjectPtr _closure_default_delegate;
    static const SQRegFunction _closure_default_delegate_funcz[];
    SQObjectPtr _thread_default_delegate;
    static const SQRegFunction _thread_default_delegate_funcz[];
    SQObjectPtr _class_default_delegate;
    static const SQRegFunction _class_default_delegate_funcz[];
    SQObjectPtr _instance_default_delegate;
    static const SQRegFunction _instance_default_delegate_funcz[];
    SQObjectPtr _weakref_default_delegate;
    static const SQRegFunction _weakref_default_delegate_funcz[];
    SQObjectPtr _userdata_default_delegate;
    static const SQRegFunction _userdata_default_delegate_funcz[];

    SQCOMPILERERROR _compilererrorhandler;
    SQPRINTFUNCTION _printfunc;
    SQPRINTFUNCTION _errorfunc;
    bool _debuginfo;
    bool _notifyallexceptions;
    bool _lineInfoInExpressions;
    bool _varTraceEnabled;
    SQUnsignedInteger compilationOptions;
    SQUnsignedInteger defaultLangFeatures;
    SQUserPointer _foreignptr;
    SQRELEASEHOOK _releasehook;
private:
    SQChar *_scratchpad;
    SQInteger _scratchpadsize;
};

#define _sp(s) (_sharedstate->GetScratchPad(s))
#define _spval (_sharedstate->GetScratchPad(-1))

#define _table_ddel     _table(_sharedstate->_table_default_delegate)
#define _array_ddel     _table(_sharedstate->_array_default_delegate)
#define _string_ddel    _table(_sharedstate->_string_default_delegate)
#define _number_ddel    _table(_sharedstate->_number_default_delegate)
#define _generator_ddel _table(_sharedstate->_generator_default_delegate)
#define _closure_ddel   _table(_sharedstate->_closure_default_delegate)
#define _thread_ddel    _table(_sharedstate->_thread_default_delegate)
#define _class_ddel     _table(_sharedstate->_class_default_delegate)
#define _instance_ddel  _table(_sharedstate->_instance_default_delegate)
#define _weakref_ddel   _table(_sharedstate->_weakref_default_delegate)
#define _userdata_ddel  _table(_sharedstate->_userdata_default_delegate)

bool CompileTypemask(SQIntVec &res,const SQChar *typemask);


#endif //_SQSTATE_H_
