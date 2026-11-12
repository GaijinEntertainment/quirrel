from "deep" import is_relaxed_equal, is_strict_equal, copy, copy_graph
from "math" import sqrt, FLT_MAX

function errorOf(f) {
  try
    f()
  catch (e)
    return e
  return null
}

println(is_relaxed_equal(1, 1.0), is_relaxed_equal(1, 2), is_relaxed_equal("a", "a"), is_relaxed_equal(null, false))
println(is_relaxed_equal({ a = [1, { b = "x" }] }, { a = [1, { b = "x" }] }), is_relaxed_equal({ a = [1, { b = "x" }] }, { a = [1, { b = "y" }] }))
println(is_relaxed_equal([1, 2], [1, 2, 3]), is_relaxed_equal({ a = 1 }, { b = 1 }), is_relaxed_equal([1], { [0] = 1 }))
println(is_relaxed_equal({ w = errorOf.weakref() }, { w = errorOf }), is_relaxed_equal(errorOf.weakref(), errorOf))

// a len slot is called, as the script's val.len() does
println(is_relaxed_equal({ len = @() 1 }, { len = @() 1 }), errorOf(@() is_relaxed_equal({ len = 1 }, { len = 1 })))

class Pt {
  x = 0
  constructor(v) { this.x = v }
  function isEqual(o) { return o instanceof this.getclass() && o.x == this.x }
}
class Raw {
  x = 0
  constructor(v) { this.x = v }
}
class Ordered { function _cmp(_o) { return 0 } }
println(is_relaxed_equal(Pt(1), Pt(1)), is_relaxed_equal({ p = Pt(1) }, { p = Pt(2) }), is_relaxed_equal(Raw(1), Raw(1)), is_relaxed_equal(Ordered(), Ordered()))
let len0 = @() 0
println(is_relaxed_equal(Pt, Pt), errorOf(@() is_relaxed_equal(Pt, Raw)), is_relaxed_equal(class { len = len0 }, class { len = len0 }))

class Frozen {
  function isEqual(_o) { return getobjflags(this) }
}
println(is_relaxed_equal(freeze({ f = Frozen() }), { f = Frozen() }), is_relaxed_equal({ f = Frozen() }, { f = Frozen() }))

let byClass = { [Raw] = @(a, b) a.x == b.x }
let byPred = { [@(v) v instanceof Raw] = @(a, b) a.x == b.x }
let byType = { float = @(a, b) (a - b) < 0.5 && (b - a) < 0.5 }
println(is_relaxed_equal(Raw(1), Raw(1), byClass), is_relaxed_equal(Raw(1), Raw(2), byPred), is_relaxed_equal([1.0, 2.0], [1.2, 2.1], byType))
println(is_relaxed_equal(5, 6, { integer = @(_a, _b) "raw" }), is_relaxed_equal({ r = Raw(1) }, freeze({ r = Raw(1) }), { [Raw] = @(_a, b) getobjflags(b) }))
println(errorOf(@() is_relaxed_equal(Raw(1), Raw(1), null)), is_relaxed_equal(1, 2, null))
println(is_relaxed_equal(Pt(1), Pt(2), { instance = @(_a, _b) true }), is_relaxed_equal(Raw(1), Raw(1), byType))

let nan = sqrt(-1.0), inf = FLT_MAX * 2.0
println(is_strict_equal(1, 1), is_strict_equal(1, 1.0), is_strict_equal(0.0, -0.0), is_strict_equal(nan, nan), is_strict_equal(inf, inf))
println(is_strict_equal({ a = [1, 0.5, "s", null] }, { a = [1, 0.5, "s", null] }), is_strict_equal({ a = [1] }, { a = [1.0] }), is_strict_equal([true], [1]))
println(is_relaxed_equal([0.0], [-0.0]), is_relaxed_equal([nan], [nan]), is_strict_equal([1.0], [1.2], byType), is_strict_equal(Pt(1), Pt(1)))

class Thrower {
  function isEqual(_o) { throw "boom" }
}
println(errorOf(@() is_relaxed_equal([Thrower()], [Thrower()])))

let calls = [0]
class Counter {
  function isEqual(_o) { calls[0]++; return true }
}
let c1 = [Counter(), null], c2 = [Counter(), null]
c1[1] = c1
c2[1] = c2
println(errorOf(@() is_relaxed_equal(c1, c2)), calls[0])

let src = { a = [1, { b = 2 }], s = "x" }
let c = copy(src)
println(c.a != src.a, c.a[1] != src.a[1], c.a[1].b, c.s, copy(5), copy(null))

class V {
  x = 0
  constructor(v) { this.x = v }
  function _deepclone(_cloner) { return clone this }
}
let v = V(1)
let sh = [1]
let d = copy({ v, sh1 = sh, sh2 = sh })
println(d.v == v, d.sh1 != sh, d.sh1 != d.sh2, errorOf(@() copy(sh, { shared = true })))

let g = copy_graph({ v, w = v, sh1 = sh, sh2 = sh })
println(g.v != v, g.v == g.w, g.v.x, g.sh1 != sh, g.sh1 == g.sh2)

class Box {
  items = null
  constructor(i) { this.items = i }
  function _deepclone(cloner) {
    let r = clone this
    r.items = cloner(this.items)
    return r
  }
}
let b = Box(sh)
let gb = copy_graph({ b, sh })
println(gb.b.items == gb.sh, gb.sh != sh)
let loop = Box(null)
loop.items = loop
println(errorOf(@() copy_graph(loop)))

let cyc = {}
cyc.self <- cyc
let cc = copy_graph(cyc)
println(cc.self == cc, cc != cyc)

println(getobjflags(copy(freeze({ a = 1 }))), getobjflags(copy_graph(freeze([1]))))
println(copy({ m = { map = @(f) f(sh) != sh } }).m, errorOf(@() copy({ k = V })), errorOf(@() copy(V)))
class Mapped { function map(f) { return f([7])[0] } }
println(copy(Mapped), copy({ k = Mapped }).k)
let skip = { map = function(_f) { throw null } }
println("m" in copy({ a = 1, m = skip }), copy([1, skip]).len())
let plain = Raw(1)
println(copy_graph({ k = V }).k == V, copy_graph(plain) == plain)
class NullClone { function _deepclone(_cloner) { throw null } }
try { copy_graph({ n = NullClone() }); println("skipped") } catch (e) println("throws", e)

local target = { t = 1 }
let dw = copy({ w = target.weakref() }), da = copy([target.weakref()])
let gw = copy_graph({ w = target.weakref() }), ga = copy_graph([target.weakref()])
println(dw.w != target, dw.w.t, gw.w == target, da[0] != target, ga[0] == target)
target = null
println(dw.w.t, gw.w, da[0].t, ga[0])

function nest(n, leaf = null) {
  local r = leaf ?? []
  for (local i = 0; i < n; i++)
    r = [r]
  return r
}
println(errorOf(@() copy(nest(49))), errorOf(@() copy(nest(50))), errorOf(@() copy(nest(49, 1))), errorOf(@() copy(nest(50, 1))))
println(errorOf(@() copy_graph(nest(999))), errorOf(@() copy_graph(nest(1000))))
local boxes = null
for (local i = 0; i < 50; i++)
  boxes = Box(boxes)
println(errorOf(@() copy_graph(boxes.items)), errorOf(@() copy_graph(boxes)))
let link = @(next) { map = @(f) f(next) }
local links = null
for (local i = 0; i < 49; i++)
  links = link(links)
println(errorOf(@() copy(links)), errorOf(@() copy(link(links))))
local src500 = nest(500), copy500 = copy_graph(src500), copied = 0
for (; src500.len(); src500 = src500[0], copy500 = copy500[0])
  copied += src500 != copy500 ? 1 : 0
println(copied, copy500.len())
