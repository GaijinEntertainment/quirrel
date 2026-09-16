class Widget {
  ready = false
  constructor() { this.ready = true }
}
let viaCall = Widget()
let viaInstance = Widget.instance()
println("viaCall.ready =", viaCall.ready)
println("viaInstance.ready =", viaInstance.ready)
