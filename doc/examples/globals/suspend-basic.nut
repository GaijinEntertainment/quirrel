let worker = newthread(function() {
  let reply = suspend("waiting")
  println("worker got", reply)
})

println("worker.call() =", worker.call())   // runs until suspend(), returns "waiting"
worker.wakeup("go")                          // resumes; suspend() returns "go" inside the thread
