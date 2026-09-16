from "debug" import seterrorhandler

seterrorhandler(function(err) {
  println("handler saw:", err)
})

// the error still propagates to the caller as a normal exception
let worker = newthread(function() { throw "ammo depleted" })
try { worker.call() } catch (e) { println("caller also caught:", e) }
