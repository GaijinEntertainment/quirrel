let f = compilestring("return 1 + 2")
println("f() =", f())

// bindings supply names visible inside the compiled code, baked in at compile time
let bindings = {FOO = 42}
let g = compilestring("return FOO", "g-src", bindings)
println("g() =", g())
bindings.FOO = 0    // too late: g already carries its own copy of the value
println("g() =", g())

try {
  compilestring("this is not )( valid squirrel")
} catch (e) {
  println("compilestring(...) throws:", e)
}
