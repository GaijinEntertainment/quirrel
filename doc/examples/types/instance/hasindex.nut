class Foo {
  x = 1
  function bar() { return 2 }
}
let f = Foo()
println("f.hasindex(\"x\") =", f.hasindex("x"))
println("f.hasindex(\"bar\") =", f.hasindex("bar"))
println("f.hasindex(\"z\") =", f.hasindex("z"))
