// Calls in every shape: script functions with argument counts, defaults, types, varargs, a bound env, generators, methods, errors,
// deep recursion, stack info, and natives. The call under test keeps its result in a local: a returned call is a tail call, not
// _OP_CALL, but a returned call of a native that is not fastcall falls through _OP_TAILCALL into the _OP_CALL body.
// Each case runs again under a debug hook and prints the second result only when it differs.
// It enters through map: a native call starts the interpreter loop again and picks its hook variant, and f.call() does not.
from "debug" import setdebughook, getstackinfos, collectgarbage

local hookFired = false
function run(f, hooked) {
  hookFired = false
  if (hooked)
    setdebughook(function(event, _src, _line, _fname) { if (event == 'c') hookFired = true })
  local text
  try {
    let r = [null].map(function(_) { let res = f(); return res })[0]
    text = $"ok {typeof r == "array" ? ",".join(r.map(@(v) $"{v}")) : r}"
  } catch (e) {
    text = $"error: {e}"
  }
  setdebughook(null)
  assert(hookFired == hooked, "the debug hook got no call event")
  return text
}

function show(name, f) {
  let text = run(f, false)
  let hooked = run(f, true)
  println(text == hooked ? $"{name}: {text}" : $"{name}: {text}; under a debug hook: {hooked}")
}

function add(a, b) { return a + b }
function nothing() { return }
function withDefault(a, b = 10) { return a + b }
function typed(a: int) { return a * 2 }
function vararg(...) { return vargv.len() }
function two(a, _b) { return a }
function depth(n) { return n == 0 ? 0 : 1 + depth(n - 1) }
function makeCounter(x) { local y = x * 2; return function() { y++; return y } }
function inner() { throw "bad" }
function outer() { return inner() + 1 }
function where() { let s = getstackinfos(2); return $"{s.func}:{s.line}" }
function caller() {
  let r = where()
  return r
}
function thrower(n) {
  if (n == 0)
    throw "zero"
  return thrower(n - 1) + 1
}
function gen() { yield 1; yield 2 }
class Point {
  x = 0
  constructor(x_) { this.x = x_ }
  function getX() { return this.x }
}

show("two arguments", function() { let r = add(2, 3); return r })
show("result not used", function() { local c = 0; let inc = function() { c++ }; inc(); inc(); return c })
show("return with no value", function() { let r = nothing(); return r })
show("default param, all passed", function() { let r = withDefault(1, 2); return r })
show("default param, one missing", function() { let r = withDefault(1); return r })
show("typed param", function() { let r = typed(4); return r })
show("typed param, wrong type", function() { let r = typed("x"); return r })
show("vararg", function() { let r = vararg(1, 2, 3); return r })
show("too few arguments", function() { let r = two(1); return r })
show("too many arguments", function() { let r = add(1, 2, 3); return r })
let env = { k = 7 }
show("bound env", function() { let r = function() { return this.k }.bindenv(env)(); return r })
show("generator", function() { let g = gen(); return [resume g, resume g] })
show("method this", function() { let o = { k = 5, m = function() { return this.k } }; let r = o.m(); return r })
show("class method", function() { let r = Point(3).getX(); return r })
show("deep recursion", @() depth(10000))
show("outers closed at return", function() { let c = makeCounter(4); c(); return c() })
show("error in a nested call", @() outer())
show("caller line", @() caller())
show("throw through calls", function() { try { thrower(5) } catch (e) { return e } })
show("null call of a closure", function() { let f = @(x) x + 1; return f?(1) })
show("null call of null", function() { let f = null; return f?(1) })
show("callee dropped during the call", function() {
  local h = null
  h = function() { h = null; collectgarbage(); return "alive" }
  let r = h()
  return r
})
show("native result", @() [3, 1, 2].len())
show("native result not used", function() { let a = [1]; a.append(2); a.append(3); return a.len() })
show("native result not kept", function() {
  local t = {}
  let w = t.weakref()
  let a = [t]
  t = null
  a.top()
  a.clear()
  return w.ref() == null
})
show("native error", @() [].pop())
show("native wrong argument count", @() [].append())
show("native tail call", @() add.call(null, 4, 5))
show("native calls back", @() [1, 2].map(@(v) add(v, 10)))
show("native closure in a local", function() { let t = getroottable()["type"]; let r = t(1); return r })
function dropF(h) { h.f = null; return 1 }
show("native dropped during the call", function() {
  let holder = { f = getroottable()["type"].bindenv({}) }
  let r = holder.f(dropF(holder))
  return $"{r} {holder.f}"
})
show("under a debug hook", function() {
  local calls = 0
  local returns = 0
  setdebughook(function(event, _src, _line, fname) {
    if (fname == "add")
      if (event == 'c')
        calls++
      else if (event == 'r')
        returns++
  })
  // add runs in the frame of viaTail, which then owes two return events
  function viaTail(a, b) { return add(a, b) }
  let r = [null].map(function(_) { let x = add(1, add(2, 3)) + viaTail(4, 5); return x })[0]
  setdebughook(null)
  return $"{r} calls {calls} returns {returns}"
})
show("debug hook set in the same loop", function() {
  local calls = 0
  setdebughook(function(event, _src, _line, fname) { if (event == 'c' && fname == "add") calls++ })
  let r = add(1, 2)
  setdebughook(null)
  return $"{r} calls {calls}"
})
