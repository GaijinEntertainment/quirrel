let co = newthread(function() {
  println("first")
  let x = suspend("paused")     // pauses the thread and hands "paused" to the caller
  println("resumed with", x)
})

println("co.call() =", co.call())          // runs until suspend(), returns suspend's argument
println("co.wakeup(\"hello\") =", co.wakeup("hello")) // resumes; suspend() returns "hello" inside the thread
println("co.getstatus() =", co.getstatus())
