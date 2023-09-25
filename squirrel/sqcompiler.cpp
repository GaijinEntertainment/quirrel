/*
    see copyright notice in squirrel.h
*/
#include "sqpcheader.h"
#ifndef NO_COMPILER
#include <stdarg.h>
#include <ctype.h>
#include <setjmp.h>
#include <algorithm>
#include "sqopcodes.h"
#include "sqstring.h"
#include "sqfuncproto.h"
#include "sqcompiler.h"
#include "sqfuncstate.h"
#include "sqoptimizer.h"
#include "sqlexer.h"
#include "sqvm.h"
#include "sqtable.h"
#include "sqast.h"
#include "sqastparser.h"
#include "sqastrender.h"
#include "sqastcodegen.h"
#include "optimizations/closureHoisting.h"
#include "sqbinaryast.h"
#include "sqcompilationcontext.h"
#include "static_analyser/analyser.h"

using namespace SQCompilation;

RootBlock *ParseToAST(Arena *astArena, SQVM *vm, const char *sourceText, size_t sourceTextSize, const SQChar *sourcename, bool raiseerror) {
  SQCompilationContext ctx(vm, astArena, sourcename, sourceText, sourceTextSize, raiseerror);
  SQParser p(vm, sourceText, sourceTextSize, sourcename, astArena, ctx);

  RootBlock *r = p.parse();

  if (r) {
    ClosureHoistingOpt opt(_ss(vm), astArena);
    opt.run(r, sourcename);
  }

  return r;
}


bool CompileWithAst(SQVM *vm, const char *sourceText, size_t sourceTextSize, const HSQOBJECT *bindings, const SQChar *sourcename, SQObjectPtr &out, bool raiseerror, bool lineinfo)
{
    if (vm->_on_compile_file)
      vm->_on_compile_file(vm, sourcename);

    Arena astArena(_ss(vm)->_alloc_ctx, "AST");

    RootBlock *r = ParseToAST(&astArena, vm, sourceText, sourceTextSize, sourcename, raiseerror);

    if (!r)
      return false;

    Arena cgArena(_ss(vm)->_alloc_ctx, "Codegen");
    SQCompilationContext ctx(vm, &cgArena, sourcename, sourceText, sourceTextSize, raiseerror);
    CodegenVisitor codegen(&cgArena, bindings, vm, sourcename, ctx, lineinfo);

    return codegen.generate(r, out);
}

bool Compile(SQVM *vm, const char *sourceText, size_t sourceTextSize, const HSQOBJECT *bindings, const SQChar *sourcename, SQObjectPtr &out, bool raiseerror, bool lineinfo) {
    return CompileWithAst(vm, sourceText, sourceTextSize, bindings, sourcename, out, raiseerror, lineinfo);
}

bool TranslateASTToBytecode(SQVM *vm, SqAstNode *ast, const HSQOBJECT *bindings, const SQChar *sourcename, const char *sourceText, size_t sourceTextSize, SQObjectPtr &out, bool raiseerror, bool lineinfo)
{
    Arena cgArena(_ss(vm)->_alloc_ctx, "Codegen");
    SQCompilationContext ctx(vm, &cgArena, sourcename, sourceText, sourceTextSize, raiseerror);
    CodegenVisitor codegen(&cgArena, bindings, vm, sourcename, ctx, lineinfo);

    assert(ast->op() == TO_BLOCK && ast->asStatement()->asBlock()->isRoot());

    return codegen.generate((RootBlock *)ast, out);
}

void AnalyseCode(SQVM *vm, SqAstNode *ast, const HSQOBJECT *bindings, const SQChar *sourcename, const char *sourceText, size_t sourceTextSize)
{
    Arena saArena(_ss(vm)->_alloc_ctx, "Analyser");
    SQCompilationContext ctx(vm, &saArena, sourcename, sourceText, sourceTextSize, true);

    StaticAnalyser sa(ctx);

    assert(ast->op() == TO_BLOCK && ast->asStatement()->asBlock()->isRoot());

    sa.runAnalysis((RootBlock *)ast);
}

bool TranslateBinaryASTToBytecode(SQVM *vm, const uint8_t *buffer, size_t size, const HSQOBJECT *bindings, SQObjectPtr &out, bool raiseerror, bool lineinfo) {
    Arena astArena(_ss(vm)->_alloc_ctx, "AST");

    MemoryInputStream mis(buffer, size);
    SQASTReader reader(_ss(vm)->_alloc_ctx, vm, &astArena, &mis, raiseerror);
    const SQChar *sourcename = NULL;
    RootBlock *r = reader.readAst(sourcename);

    if (!r) {
      return false;
    }

    return TranslateASTToBytecode(vm, r, bindings, sourcename, NULL, 0, out, raiseerror, lineinfo);
}

bool ParseAndSaveBinaryAST(SQVM *vm, const char *sourceText, size_t sourceTextSize, const SQChar *sourcename, OutputStream *ostream, bool raiseerror) {
    Arena astArena(_ss(vm)->_alloc_ctx, "AST");

    RootBlock *r = ParseToAST(&astArena, vm, sourceText, sourceTextSize, sourcename, raiseerror);

    if (!r) {
        return false;
    }

    SQASTWriter writer(_ss(vm)->_alloc_ctx, ostream);
    writer.writeAST(r, sourcename);

    return true;
}

#endif
