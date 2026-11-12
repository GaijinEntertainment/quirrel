from "async" import Future

let p = Future()
println("p.getState() =", p.getState())            // pending

class Twice(Future) {
  constructor() {
    Future.constructor.call(this)
    Future.constructor.call(this)    // constructing twice on the same instance
  }
}
try { Twice() } catch (e) { println("Twice() throws:", e) }
