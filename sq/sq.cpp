/*  see copyright notice in squirrel.h */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <string>
#include <vector>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#include <conio.h>
#endif
#include <squirrel.h>
#include <sqstdblob.h>
#include <sqstdsystem.h>
#include <sqstddatetime.h>
#include <sqstdio.h>
#include <sqstdmath.h>
#include <sqstdstring.h>
#include <sqstdaux.h>
#include <sqmodules.h>
#include <sqio.h>
#include <sqstddebug.h>
#include <squirrel/sqtypeparser.h>

#define scvprintf vfprintf


static SqModules *module_mgr = nullptr;
static bool check_stack_mode = false;


void PrintVersionInfos();

SQInteger quit(HSQUIRRELVM v)
{
    int *done;
    sq_getuserpointer(v,-1,(SQUserPointer*)&done);
    *done=1;
    return 0;
}

void printfunc(HSQUIRRELVM SQ_UNUSED_ARG(v),const SQChar *s,...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(stdout, s, vl);
    va_end(vl);
}

static FILE *errorStream = stderr;

void errorfunc(HSQUIRRELVM SQ_UNUSED_ARG(v),const SQChar *s,...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(errorStream, s, vl);
    va_end(vl);
}

void PrintVersionInfos()
{
    fprintf(stdout,_SC("%s %s (%d bits)\n"),SQUIRREL_VERSION,SQUIRREL_COPYRIGHT,((int)(sizeof(SQInteger)*8)));
}

void PrintUsage()
{
    fprintf(stderr,_SC("Usage: sq <scriptpath [args]> [options]\n")
        _SC("Available options are:\n")
        _SC("  -c                        compiles the file to bytecode (default output 'out.cnut')\n")
        _SC("  -o <out-file>             specifies output file for the -c option\n")
        _SC("  -ast-dump [out-file]      dump AST into console or file if specified\n")
        _SC("  -absolute-path            use absolute path when print diangostics\n")
        _SC("  -bytecode-dump [out-file] dump SQ bytecode into console or file if specified\n")
        _SC("  -diag-file file           write diagnostics into specified file\n")
        _SC("  -sa                       enable static analyzer\n")
        _SC("  --check-stack             check stack after each script execution\n")
        _SC("  --warnings-list           print all warnings and exit\n")
        _SC("  --parse-types             parse function types from file\n")
        _SC("  --D:<diagnostic-name>     disable diagnostic by text id\n")
        _SC("  -optCH                    enable Closure Hoisting Optimization\n")
        _SC("  -d                        generates debug infos\n")
        _SC("  -v                        displays version\n")
        _SC("  -h                        prints help\n"));
}

std::string read_file_ignoring_utf8bom(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
        return std::string();

    int length = 0;
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::string result;
    result.resize(length + 1);
    size_t sz = fread(&result[0], 1, length, f);
    fclose(f);
    result[sz] = 0;

    if (sz >= 3 && result[0] == 0xEF && result[1] == 0xBB && result[2] == 0xBF)
        result.erase(0, 3);

    return result;
}

static int fill_stack(HSQUIRRELVM v)
{
  if (!check_stack_mode)
    return 0;

  for (int i = 0; i < 8; i++)
    sq_pushinteger(v, i + 1000000);

  return sq_gettop(v);
}

static bool check_stack(HSQUIRRELVM v, int expected_top)
{
  if (!check_stack_mode)
    return true;

  int top = sq_gettop(v);
  if (top != expected_top)
  {
    fprintf(errorStream, "ERROR: Stack top is %d, expected %d\n", top, expected_top);
    return false;
  }

  for (int i = 0; i < 8; i++)
  {
    SQInteger val = -1;
    if (SQ_FAILED(sq_getinteger(v, -8 + i, &val)) || val != i + 1000000)
    {
      fprintf(errorStream, "ERROR: Stack value at %d is %d, expected %d\n", -8 + i, int(val), i + 1000000);
      return false;
    }
  }

  return true;
}

struct DumpOptions {
  bool astDump;
  bool bytecodeDump;

  const char *astDumpFileName;
  const char *bytecodeDumpFileName;
};

static void dumpAst_callback(HSQUIRRELVM vm, SQCompilation::SqASTData *astData, void *opts)
{
    if (opts == NULL)
      return;
    DumpOptions *dumpOpt = (DumpOptions *)opts;
    if (dumpOpt->astDump)
    {
        if (dumpOpt->astDumpFileName)
        {
            FileOutputStream fos(dumpOpt->astDumpFileName);
            if (fos.valid())
            {
                sq_dumpast(vm, astData, &fos);
            }
            else
            {
                printf(_SC("Error: cannot open AST dump file '%s'\n"), dumpOpt->astDumpFileName);
            }
        }
        else
        {
            FileOutputStream fos(stdout);
            sq_dumpast(vm, astData, &fos);
        }
    }
}

static void dumpBytecodeAst_callback(HSQUIRRELVM vm, HSQOBJECT obj, void *opts)
{
    if (opts == NULL)
      return;
    DumpOptions *dumpOpt = (DumpOptions *)opts;
    if (dumpOpt->bytecodeDump)
    {
        if (dumpOpt->bytecodeDumpFileName)
        {
            FileOutputStream fos(dumpOpt->bytecodeDumpFileName);
            if (fos.valid())
            {
                sq_dumpbytecode(vm, obj, &fos);
            }
            else
            {
                printf(_SC("Error: cannot open Bytecode dump file '%s'\n"), dumpOpt->bytecodeDumpFileName);
            }
        }
        else
        {
            FileOutputStream fos(stdout);
            sq_dumpbytecode(vm, obj, &fos);
        }
    }
}

static bool search_sqconfig(const char * initial_file_name, char *buffer, size_t bufferSize)
{
  if (!initial_file_name)
    return false;

  size_t curSize = bufferSize;
  char *ptr = buffer;
  const char * slash1 = strrchr(initial_file_name, '\\');
  const char * slash2 = strrchr(initial_file_name, '/');
  const char * slash = slash1 > slash2 ? slash1 : slash2;

  if (slash) {
    size_t prefixSize = slash - initial_file_name + 1;
    if (prefixSize > curSize)
      return false;

    memcpy(buffer, initial_file_name, prefixSize);
    curSize -= prefixSize;
    ptr += prefixSize;
  }

  static const char configName[] = ".sqconfig";
  const size_t configNameSize = (sizeof configName) - 1;
  static const char upDir[] = "../";
  const size_t upDirSize = (sizeof upDir) - 1;

  char *dirPtr = ptr;

  memcpy(ptr, configName, configNameSize);
  ptr += configNameSize;
  curSize -= configNameSize;
  ptr[0] = '\0';

  for (int i = 0; i < 16 && curSize > 0; i++) {
    if (FILE * f = fopen(buffer, "rt")) {
      fclose(f);
      return true;
    }

    if (curSize > upDirSize) {
      memcpy(dirPtr, upDir, upDirSize);
      dirPtr += upDirSize;
      curSize -= upDirSize;
      ptr += upDirSize;
      memcpy(dirPtr, configName, configNameSize);
      ptr[0] = '\0';
    }
    else {
      break;
    }
  }

  return false;
}

static bool checkOption(char *argv[], int argc, const char *option, const char *&optArg) {
  optArg = nullptr;
  for (int i = 1; i< argc; ++i) {
    const char *arg = argv[i];

    if (arg[0] == '-' && strcmp(arg + 1, option) == 0) {
      if ((i + 1) < argc) {
        const char *arg2 = argv[i + 1];
        if (arg2[0] != '-') { // not next option
          optArg = arg2;
        }
      }
      return true;
    }
  }

  return false;
}

bool ends_with(const char * str, const char * suffix)
{
    const char * s = strstr(str, suffix);
    return s && s[strlen(suffix)] == 0;
}

static bool parse_types_from_file(HSQUIRRELVM sqvm, const char *filename)
{
    std::string code = read_file_ignoring_utf8bom(filename);
    if (code.empty())
        return false;

    const char *c = code.c_str();
    std::string line;
    int lineNum = 1;

    while (*c) {
        if (*c == '\n' || c[1] == 0) {
            if (c[1] == 0)
                line += *c;
            if (!line.empty()) {
                printf("%s\n", line.c_str());

                SQFunctionType t(_ss(sqvm));
                SQInteger errorPos;
                SQObjectPtr errorString;
                if (sq_parse_function_type_string(sqvm, line.c_str(), t, errorPos, errorString)) {
                    SQObjectPtr s = sq_stringify_function_type(sqvm, t);
                    printf("%s\n", _stringval(s));
                    printf("  functionName: %s\n", _stringval(t.functionName));
                    printf("  returnTypeMask: 0x%x\n", unsigned(t.returnTypeMask));
                    printf("  objectTypeMask: 0x%x\n", unsigned(t.objectTypeMask));
                    printf("  ellipsisArgTypeMask: 0x%x\n", unsigned(t.ellipsisArgTypeMask));
                    printf("  requiredArgs: %d\n", int(t.requiredArgs));
                    printf("  argCount: %d\n", int(t.argTypeMask.size()));
                    printf("\n");
                }
                else {
                    printf("ERROR: %s\n", _stringval(errorString));
                    printf("at %s:%d:%d\n\n", filename, lineNum, int(errorPos));
                }

                line.clear();
            }
            lineNum++;
        }
        else {
            if (*c != '\r')
                line += *c;
        }

        c++;
    }

    return true;
}



#define _INTERACTIVE 0
#define _DONE 2
#define _ERROR 3
//<<FIXME>> this func is a mess
int getargs(HSQUIRRELVM v,int argc, char* argv[],SQInteger *retval)
{
    assert(module_mgr != nullptr && "Module manager has to be existed");
    const char *optArg = nullptr;
    DumpOptions dumpOpt = { 0 };
    FILE *diagFile = nullptr;
    int compiles_only = 0;
    bool static_analysis = checkOption(argv, argc, "sa", optArg); // TODO: refact ugly loop below using this function
    bool parse_types = false;

    if (static_analysis) {
      sq_enablesyntaxwarnings(true);
    }

    std::vector<std::string> listOfNutFiles;

    const char * output = NULL;
    *retval = 0;
    if(argc>1)
    {
        int index=1, exitloop=0;

        while(index < argc && !exitloop)
        {
            const char *arg = argv[index];
            if (arg[0] == '-' && arg[1] == '-')
                arg++;

            if (strcmp("-ast-dump", arg) == 0)
            {
                dumpOpt.astDump = true;
                if ((index + 1) < argc && argv[index + 1][0] != '-' && !ends_with(argv[index + 1], ".nut"))
                    dumpOpt.astDumpFileName = argv[++index];
            }
            else if (strcmp("-absolute-path", arg) == 0)
            {
                module_mgr->compilationOptions.useAbsolutePath = true;
            }
            else if (strcmp("-parse-types", arg) == 0)
            {
                parse_types = true;
            }
            else if (strcmp("-d", arg) == 0)
            {
                module_mgr->compilationOptions.debugInfo = true;
                //sq_enabledebuginfo(v,1);
                sq_lineinfo_in_expressions(v, 1);
            }
            else if (strcmp("-diag-file", arg) == 0)
            {
                if (((index + 1) < argc) && argv[index + 1][0] != '-')
                {
                    const char *fileName = argv[++index];
                    diagFile = fopen(fileName, "wb");
                    if (diagFile == NULL)
                    {
                        printf(_SC("Cannot open diagnostic output file '%s'\n"), fileName);
                        return _ERROR;
                    }
                }
                else
                {
                    printf(_SC("-diag-file option requires file name to be specified\n"));
                    return _ERROR;
                }
            }
            else if (strcmp("-bytecode-dump", arg) == 0)
            {
              dumpOpt.bytecodeDump = 1;
              if (((index + 1) < argc) && argv[index + 1][0] != '-' && !ends_with(argv[index + 1], ".nut"))
              {
                  dumpOpt.bytecodeDumpFileName = argv[++index];
              }
            }
            else if (strcmp("-c", arg) == 0) {
                compiles_only = 1;
            }
            else if (strcmp("-o", arg) == 0) {
                index++;
                output = arg;
            }
            else if (strcmp("-check-stack", arg) == 0) {
                check_stack_mode = true;
            }
            else if (strcmp("-optCH", arg) == 0) {
                sq_setcompilationoption(v, CompilationOptions::CO_CLOSURE_HOISTING_OPT, true);
            }
            else if (strcmp("-v", arg) == 0 || strcmp("--version", arg) == 0) {
                PrintVersionInfos();
                return _DONE;
            }
            else if (strcmp("-h", arg) == 0 || strcmp("--help", arg) == 0) {
                PrintVersionInfos();
                PrintUsage();
                return _DONE;
            }
            else if (strcmp("-sa", arg) == 0) {
                static_analysis = true;
            }
            else if (static_analysis && strncmp("--D:", arg, 4) == 0) {
                if (!sq_setdiagnosticstatebyname(&arg[5], false))
                    printf("Unknown warning ID: '%s'\n", &arg[5]);
            }
            else if (strcmp(arg, "-warnings-list") == 0) {
                sq_printwarningslist(stdout);
                return _DONE;
            }
            else if (arg[0] == '-') {
                PrintVersionInfos();
                printf(_SC("unknown argument '%s'\n"), arg);
                PrintUsage();
                *retval = -1;
                return _ERROR;
            }
            else {
                listOfNutFiles.emplace_back(std::string(arg));
            }

            index++;
        }

        module_mgr->up_data = &dumpOpt;
        module_mgr->onAST_cb = &dumpAst_callback;
        module_mgr->onBytecode_cb = &dumpBytecodeAst_callback;

        module_mgr->compilationOptions.doStaticAnalysis = static_analysis;

        if (static_analysis) {
          sq_setcompilationoption(v, CompilationOptions::CO_CLOSURE_HOISTING_OPT, false);
        }

        if (diagFile)
        {
          errorStream = diagFile;
        }

        // src file

        if (listOfNutFiles.empty()) {
            printf("Error: expected source file name\n");
            return _ERROR;
        }

        if (parse_types) {
            for (const std::string & fn : listOfNutFiles) {
                if (!parse_types_from_file(v, fn.c_str()))
                    return _ERROR;
            }
            return _DONE;
        }

        for (const std::string & fn : listOfNutFiles) {
            const SQChar *filename=fn.c_str();

            if (static_analysis) {
              sq_resetanalyzerconfig();
              char buffer[1024];
              if (search_sqconfig(filename, buffer, sizeof buffer)) {
                if (!sq_loadanalyzerconfig(buffer)) {
                  fprintf(errorStream, "Cannot load .sqconfig file %s\n", buffer);
                  return _ERROR;
                }
              }
            }

            if (compiles_only) {
                if(SQ_SUCCEEDED(sqstd_loadfile(v,filename,SQTrue))){
                    const SQChar *outfile = _SC("out.cnut");
                    if(output) {
                        outfile = output;
                    }
                    if(SQ_SUCCEEDED(sqstd_writeclosuretofile(v,outfile)))
                        return _DONE;
                }
            }
            else {
                int top = fill_stack(v);

                Sqrat::Object exports;
                std::string errMsg;
                int retCode = _DONE;

                if (!module_mgr->requireModule(filename, true, static_analysis ? SqModules::__analysis__ : SqModules::__main__, exports, errMsg)) {
                    retCode = _ERROR;
                }

                if (retCode == _DONE && sq_isinteger(exports.GetObject())) {
                    *retval = exports.GetObject()._unVal.nInteger;
                }

                if (static_analysis) {
                  sq_checkglobalnames(v);
                }

                if (retCode != _DONE) {
                  fprintf(stderr, _SC("Error [%s]\n"), errMsg.c_str());
                }

                if (!check_stack(v, top)) {
                    fprintf(stderr, _SC("Stack check failed after execution of '%s'\n"), filename);
                    *retval = -3;
                    retCode = _ERROR;
                }

                return retCode;
            }

            //if this point is reached an error occurred
            {
                const SQChar *err;
                sq_getlasterror(v);
                if(SQ_SUCCEEDED(sq_getstring(v,-1,&err))) {
                    printf(_SC("Error [%s]\n"),err);
                    *retval = -2;
                    return _ERROR;
                }
            }

        }
    }

    return _INTERACTIVE;
}

void Interactive(HSQUIRRELVM v)
{

#define MAXINPUT 1024
    SQChar buffer[MAXINPUT];
    SQInteger blocks =0;
    SQInteger string=0;
    SQInteger retval=0;
    SQInteger done=0;
    PrintVersionInfos();

    HSQOBJECT bindingsTable;
    sq_newtable(v);
    sq_getstackobj(v, -1, &bindingsTable);
    sq_addref(v, &bindingsTable);

    sq_registerbaselib(v);

    sq_pushstring(v,_SC("quit"),-1);
    sq_pushuserpointer(v,&done);
    sq_newclosure(v,quit,1);
    sq_setparamscheck(v,1,NULL);
    sq_newslot(v,-3,SQFalse);

    sq_pop(v, 1); // bindingsTable

    while (!done)
    {
        SQInteger i = 0;
        printf(_SC("\nsq>"));
        for(;;) {
            int c;
            if(done)return;
            c = getchar();
            if (c == _SC('\n')) {
                if (i>0 && buffer[i-1] == _SC('\\'))
                {
                    buffer[i-1] = _SC('\n');
                }
                else if(blocks==0)break;
                buffer[i++] = _SC('\n');
            }
            else if (c==_SC('}')) {blocks--; buffer[i++] = (SQChar)c;}
            else if(c==_SC('{') && !string){
                    blocks++;
                    buffer[i++] = (SQChar)c;
            }
            else if(c==_SC('"') || c==_SC('\'')){
                    string=!string;
                    buffer[i++] = (SQChar)c;
            }
            else if (i >= MAXINPUT-1) {
                fprintf(stderr, _SC("sq : input line too long\n"));
                break;
            }
            else{
                buffer[i++] = (SQChar)c;
            }
        }
        buffer[i] = _SC('\0');

        if(buffer[0]==_SC('=')){
            scsprintf(sq_getscratchpad(v,MAXINPUT),(size_t)MAXINPUT,_SC("return (%s)"),&buffer[1]);
            memcpy(buffer,sq_getscratchpad(v,-1),(strlen(sq_getscratchpad(v,-1))+1)*sizeof(SQChar));
            retval=1;
        }
        i=strlen(buffer);
        if(i>0){
            SQInteger oldtop=sq_gettop(v);
            if(SQ_SUCCEEDED(sq_compile(v,buffer,i,_SC("interactive console"),SQTrue,&bindingsTable))){
                sq_pushroottable(v);
                if(SQ_SUCCEEDED(sq_call(v,1,retval,SQTrue)) &&  retval){
                    printf(_SC("\n"));
                    sq_pushroottable(v);
                    sq_pushstring(v,_SC("print"),-1);
                    sq_get(v,-2);
                    sq_pushroottable(v);
                    sq_push(v,-4);
                    sq_call(v,2,SQFalse,SQTrue);
                    retval=0;
                    printf(_SC("\n"));
                }
            }

            sq_settop(v,oldtop);
        }
    }

    sq_release(v, &bindingsTable);
}

int main(int argc, char* argv[])
{
    HSQUIRRELVM v;
    SQInteger retval = 0;

    v=sq_open(1024);
    sq_setprintfunc(v,printfunc,errorfunc);

    sq_pushroottable(v);

    sqstd_register_bloblib(v);
    sqstd_register_iolib(v);
    sqstd_register_systemlib(v);
    sqstd_register_datetimelib(v);
    sqstd_register_mathlib(v);
    sqstd_register_stringlib(v);
    sqstd_register_debuglib(v);

    //aux library
    //sets error handlers
    sqstd_seterrorhandlers(v);

    module_mgr = new SqModules(v);
    module_mgr->registerMathLib();
    module_mgr->registerStringLib();
    module_mgr->registerSystemLib();
    module_mgr->registerIoStreamLib();
    module_mgr->registerIoLib();
    module_mgr->registerDateTimeLib();
    module_mgr->registerDebugLib();
    module_mgr->registerModulesLib();

    sqstd_register_command_line_args(v, argc, argv);

    //gets arguments
    switch(getargs(v,argc,argv,&retval))
    {
    case _INTERACTIVE:
        Interactive(v);
        break;
    case _DONE:
    case _ERROR:
    default:
        break;
    }

    if (errorStream != stderr)
    {
      fclose(errorStream);
    }

    delete module_mgr;
    sq_close(v);

    return int(retval);
}

