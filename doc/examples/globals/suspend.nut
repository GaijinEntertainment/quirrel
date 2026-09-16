let co = newthread(function() {
  let first = suspend("ready")         // pauses; "ready" becomes call()'s return value
  let second = suspend($"got {first}") // resumed with wakeup's argument as "first"
  println("finished with", second)
})

println("co.call() =", co.call())      // runs to the first suspend()
println("co.wakeup(\"a\") =", co.wakeup("a")) // suspend() returns "a"; runs to the second suspend()
co.wakeup("b")           // suspend() returns "b"; thread runs to completion
