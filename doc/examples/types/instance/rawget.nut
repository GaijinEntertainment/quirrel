class Foo {
  x = 1
  function bar() { return 2 }
}
let f = Foo()
println("f.rawget(\"x\") =", f.rawget("x"))
println("typeof f.rawget(\"bar\") == \"function\" =", typeof f.rawget("bar") == "function")
try {
  f.rawget("missing")
} catch (e) {
  println("f.rawget(\"missing\") throws:", e)
}
