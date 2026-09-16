from "debug" import seterrorhandler

seterrorhandler(function(err) {
  println("handler saw:", err)
})

// the error reaches the handler only once it is uncaught all the way up to
// this call, then still propagates to us as a normal exception
let co = newthread(function() { throw "boom" })
try { co.call() } catch (e) { println("caller also caught:", e) }

seterrorhandler(null)
let co2 = newthread(function() { throw "boom2" })
try { co2.call() } catch (e) { println("no handler this time:", e) }
