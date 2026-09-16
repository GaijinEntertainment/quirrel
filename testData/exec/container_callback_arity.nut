// A container pushes a fixed number of callback arguments: value, index or key and
// the container, plus the accumulator for reduce. A callback that declares more
// parameters must not move the call's stack base below those pushed values.

from "test.native" import takes_four_args, takes_five_args

let arr = [10, 20, 30]
let tbl = { a = 1, b = 2, c = 3 }

// Table iteration order is randomized, so every table result is order independent.
let function sorted(a) {
  let res = clone a
  res.sort()
  return res
}
let function show(a) {
  return "[" + ", ".join(sorted(a).map(@(v) v.tostring())) + "]"
}

let function head(msg) {
  return msg.slice(0, 26)   // drops the script path the VM appends
}

let function expectArityError(name, cb) {
  try {
    cb()
    println($"{name}: no error")
  }
  catch (e) {
    println($"{name}: {head(e)}")
  }
}

println("-- every declared parameter is filled at the budget")

arr.each(function(v, i, a) { println($"array.each {v} {i} {a.len()}") })
println("array.map " + show(arr.map(function(v, i, a) { return v + i + a.len() })))
println("array.filter " + show(arr.filter(function(v, i, a) { return i < a.len() - 1 })))
println("array.findindex " + arr.findindex(function(v, i, a) { return v == 30 }))
println("array.findvalue " + arr.findvalue(function(v, i, a) { return i == 1 }))
println("array.reduce " + arr.reduce(function(acc, v, i, a) { return acc + v + i }))

let keys = []
tbl.each(function(v, k, t) { keys.append($"{k}{v}{t.len()}") })
println("table.each " + show(keys))
println("table.map " + show(tbl.map(function(v, k, t) { return v + t.len() }).values()))
println("table.filter " + show(tbl.filter(function(v, k, t) { return v > 1 }).values()))
println("table.reduce " + tbl.reduce(function(acc, v, k, t) { return acc + v }))

println("-- one parameter past the budget takes its default")

arr.each(function(v, i, a, extra = "def") { println($"array.each {v} {extra}") })
println("array.map " + show(arr.map(function(v, i, a, extra = 5) { return v + extra })))
println("array.reduce " + arr.reduce(function(acc, v, i, a, extra = 100) { return acc + extra }))
println("table.map " + show(tbl.map(function(v, k, t, extra = 7) { return v + extra }).values()))

println("-- a vararg tail past the budget stays empty")

arr.each(function(v, i, a, ...) { println($"array.each {v} {vargv.len()}") })
tbl.each(function(v, k, t, ...) { assert(vargv.len() == 0) })
println("table.each vararg ok")

println("-- one required parameter past the budget is a plain, catchable error")

expectArityError("array.each", @() arr.each(function(v, i, a, x) {}))
expectArityError("array.map", @() arr.map(function(v, i, a, x) { return v }))
expectArityError("array.apply", @() clone(arr).apply(function(v, i, a, x) { return v }))
expectArityError("array.filter", @() arr.filter(function(v, i, a, x) { return true }))
expectArityError("array.findindex", @() arr.findindex(function(v, i, a, x) { return true }))
expectArityError("array.findvalue", @() arr.findvalue(function(v, i, a, x) { return true }))
expectArityError("array.reduce", @() arr.reduce(function(acc, v, i, a, x) { return acc }))
expectArityError("table.each", @() tbl.each(function(v, k, t, x) {}))
expectArityError("table.map", @() tbl.map(function(v, k, t, x) { return v }))
expectArityError("table.filter", @() tbl.filter(function(v, k, t, x) { return true }))
expectArityError("table.findindex", @() tbl.findindex(function(v, k, t, x) { return true }))
expectArityError("table.findvalue", @() tbl.findvalue(function(v, k, t, x) { return true }))
expectArityError("table.reduce", @() tbl.reduce(function(acc, v, k, t, x) { return acc }))

println("-- a native callback is clamped the same way")

// takes_four_args and takes_five_args are one native bound under two fixed
// parameter counts; each returns the stack top it was called with, so a shifted
// frame shows up as a wrong count instead of a wrong value.
println("array.reduce native " + arr.reduce(takes_four_args))
println("table.reduce native " + tbl.reduce(takes_four_args))

expectArityError("array.each native", @() arr.each(takes_four_args))
expectArityError("array.map native", @() arr.map(takes_four_args))
expectArityError("array.apply native", @() clone(arr).apply(takes_four_args))
expectArityError("array.filter native", @() arr.filter(takes_four_args))
expectArityError("array.findindex native", @() arr.findindex(takes_four_args))
expectArityError("array.findvalue native", @() arr.findvalue(takes_four_args))
expectArityError("array.reduce native", @() arr.reduce(takes_five_args))
expectArityError("table.each native", @() tbl.each(takes_four_args))
expectArityError("table.map native", @() tbl.map(takes_four_args))
expectArityError("table.filter native", @() tbl.filter(takes_four_args))
expectArityError("table.findindex native", @() tbl.findindex(takes_four_args))
expectArityError("table.findvalue native", @() tbl.findvalue(takes_four_args))
expectArityError("table.reduce native", @() tbl.reduce(takes_five_args))

println("-- the iteration survives the error and the containers are untouched")

println("array " + show(arr))
println("table " + show(tbl.values()))
