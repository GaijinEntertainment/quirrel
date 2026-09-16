// BUG: sqbaselib.cpp base_newthread() accepts native closures via its typemask
// but unconditionally does _closure(func)->_function => reads garbage / crashes.
print("calling newthread(print)...\n")
local err = null
try {
  local t = newthread(print)
  println($"newthread returned: {typeof(t)}")
} catch (e) {
  err = e
}
println($"error: {err}")
print("SURVIVED (no crash)\n")
