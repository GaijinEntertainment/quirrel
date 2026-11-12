// A native that only a captured local holds, and that its own callback releases: the frame holds the last reference, so
// the native is freed when the frame is left. A sanitizer or a debug heap shows a read of the native after that point.
function test() {
  let t = { a = 1 }
  local f = t.each.bindenv(t)
  let w = f.weakref()
  let r = f(function(_k, _v) { f = null })
  return $"result {r}, native freed: {w.ref() == null}"
}

// the type check of each sees the this of the call site, which must be a table
println(test.call({}))
