from "debug" import getstackinfos

function inner(x) {
  println("level 0 func:", getstackinfos(0).func) // getstackinfos itself
  let si1 = getstackinfos(1)
  println("level 1 func:", si1.func)               // inner, our caller
  println("\"x\" in si1.locals =", "x" in si1.locals)
}

inner(5)
println("getstackinfos(100) =", getstackinfos(100)) // no such level
