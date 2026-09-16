from "async" import Future

let bounty = Future()
bounty.resolve(100)

println("bounty.getValue() =", bounty.getValue())
