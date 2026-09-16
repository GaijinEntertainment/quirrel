// BUG: sqbaselib.cpp closure_call() reads stack_get(v,-1) (the LAST ARGUMENT)
// instead of slot 1 (the callee) to decide whether to tail-call.
// So Class.call(...) with a closure as last arg wrongly takes the tail-call path.

function countArgs(...) {
  return vargv.len()
}

local sideEffects = []

function sideEffectClosure(name) {
  return function() {
    sideEffects.append(name)
    return name
  }
}

function sideEffectsSummary() {
  if (sideEffects.len() == 0)
    return "none"

  return ",".join(sideEffects)
}

println($"closure.call 0 args: {countArgs.call(null)}")
println($"closure.call 1 closure arg: {countArgs.call(null, sideEffectClosure("closure-1"))}")
println($"closure.call 2 args closure last: {countArgs.call(null, 1, sideEffectClosure("closure-2-last"))}")
println($"closure.call 2 closure args: {countArgs.call(null, sideEffectClosure("closure-2-a"), sideEffectClosure("closure-2-b"))}")
println($"closure.call 3 args closure last: {countArgs.call(null, 1, "x", sideEffectClosure("closure-3-last"))}")
println($"closure.call 3 closure args: {countArgs.call(null, sideEffectClosure("closure-3-a"), sideEffectClosure("closure-3-b"), sideEffectClosure("closure-3-c"))}")

class C {
  argc = null
  lastType = null

  constructor(...) {
    this.argc = vargv.len()
    this.lastType = vargv.len() > 0 ? typeof(vargv[vargv.len() - 1]) : "none"
  }
}

local inst = C.call(null)
println($"class.call 0 args: argc={inst.argc} last={inst.lastType}")

inst = C.call(null, sideEffectClosure("class-1"))
println($"class.call 1 closure arg: argc={inst.argc} last={inst.lastType}")

inst = C.call(null, 1, sideEffectClosure("class-2-last"))
println($"class.call 2 args closure last: argc={inst.argc} last={inst.lastType}")

inst = C.call(null, sideEffectClosure("class-2-a"), sideEffectClosure("class-2-b"))
println($"class.call 2 closure args: argc={inst.argc} last={inst.lastType}")

inst = C.call(null, sideEffectClosure("class-2-first"), 1)
println($"class.call 2 args int last: argc={inst.argc} last={inst.lastType}")

inst = C.call(null, 1, "x", sideEffectClosure("class-3-last"))
println($"class.call 3 args closure last: argc={inst.argc} last={inst.lastType}")

inst = C.call(null, sideEffectClosure("class-3-a"), sideEffectClosure("class-3-b"), sideEffectClosure("class-3-c"))
println($"class.call 3 closure args: argc={inst.argc} last={inst.lastType}")

function gen(...) {
  yield vargv.len()
}

local g = gen.call(null)
println($"gen.call 0 args: {typeof(g)}")

g = gen.call(null, sideEffectClosure("gen-1"))
println($"gen.call 1 closure arg: {typeof(g)}")

g = gen.call(null, 1, sideEffectClosure("gen-2-last"))
println($"gen.call 2 args closure last: {typeof(g)}")

g = gen.call(null, sideEffectClosure("gen-2-a"), sideEffectClosure("gen-2-b"))
println($"gen.call 2 closure args: {typeof(g)}")

g = gen.call(null, 1, "x", sideEffectClosure("gen-3-last"))
println($"gen.call 3 args closure last: {typeof(g)}")

g = gen.call(null, sideEffectClosure("gen-3-a"), sideEffectClosure("gen-3-b"), sideEffectClosure("gen-3-c"))
println($"gen.call 3 closure args: {typeof(g)}")

println($"argument closure side effects: {sideEffectsSummary()}")
