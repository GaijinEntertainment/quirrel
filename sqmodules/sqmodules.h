/*
  This is not a production code, but a demonstration of concept of modules
*/


#pragma once

#include <squirrel.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "sqratLite.h"

class SqModules
{
public:
  template <class T> using vector = SQRAT_STD::vector<T>;
  using string = Sqrat::string;

  struct Module
  {
    string fn;
    Sqrat::Object exports;
    Sqrat::Object stateStorage;
    Sqrat::Object refHolder;
    Sqrat::Object moduleThis;
    string  __name__;
  };

public:
  SqModules(HSQUIRRELVM vm)
    : sqvm(vm), onAST_cb(nullptr), onBytecode_cb(nullptr), up_data(nullptr)
  {
    compilationOptions.useAST = false;
    compilationOptions.raiseError = true;
    compilationOptions.debugInfo = false;
    compilationOptions.doStaticAnalysis = false;
  }

  HSQUIRRELVM getVM() { return sqvm; }

  // File name may be:
  // 1) relative to currently running script
  // 2) relative to base path
  // Note: accessing files outside of current base paths (via ../../)
  // can mess things up (breaking load module once rule)
  // __name__ is put to module this. Use __fn__ constant to use resolved filename or custom string to override
  bool  requireModule(const char *fn, bool must_exist, const char *__name__, Sqrat::Object &exports, string &out_err_msg);
  // This can also be used for initial module execution
  bool  reloadModule(const char *fn, bool must_exist, const char *__name__, Sqrat::Object &exports, string &out_err_msg);

  bool  reloadAll(string &err_msg);

  bool  addNativeModule(const char *module_name, const Sqrat::Object &exports);

  void  registerMathLib();
  void  registerStringLib();
  void  registerSystemLib();
  void  registerIoStreamLib();
  void  registerIoLib();
  void  registerDateTimeLib();

  Sqrat::Object *findNativeModule(const string &module_name);

  void  bindRequireApi(HSQOBJECT bindings);
  void  bindBaseLib(HSQOBJECT bindings);

private:
  // Script API
  //   require(file_name, must_exist=true)
  template<bool must_exist> static SQInteger sqRequire(HSQUIRRELVM vm);

  void  resolveFileName(const char *fn, string &res);
  bool  checkCircularReferences(const char *resolved_fn, const char *orig_fn);
  enum class CompileScriptResult
  {
    Ok,
    FileNotFound,
    CompilationFailed
  };
  CompileScriptResult compileScript(const char *resolved_fn, const char *orig_fn, const HSQOBJECT *bindings,
                                    Sqrat::Object &script_closure, string &out_err_msg);
  bool compileScriptImpl(const std::vector<char> &buf, const char *sourcename, const HSQOBJECT *bindings);

  Sqrat::Object  setupStateStorage(const char *resolved_fn);
  Module * findModule(const char * resolved_fn);

  typedef SQInteger (*RegFunc)(HSQUIRRELVM);
  void  registerStdLibNativeModule(const char *name, RegFunc);


public:
  static const char *__main__, *__fn__;

  struct {
    bool useAST;
    bool raiseError;
    bool debugInfo;
    bool doStaticAnalysis;
  } compilationOptions;

  void *up_data;
  void (*onAST_cb)(HSQUIRRELVM, SqAstNode *, void *);
  void (*onBytecode_cb)(HSQUIRRELVM, HSQOBJECT, void *);

private:
  std::vector<Module>  modules;
  std::vector<Module>  prevModules; //< for hot reload

  std::unordered_map<string, Sqrat::Object> nativeModules;
  std::vector<string>  runningScripts;
  HSQUIRRELVM sqvm = nullptr;
};


inline Sqrat::Object *SqModules::findNativeModule(const string &module_name)
{
  auto it = nativeModules.find(module_name);
  return (it == nativeModules.end()) ? nullptr : &it->second;
}
