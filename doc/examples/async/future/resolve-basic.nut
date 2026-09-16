from "async" import Future

let quest = Future()
quest.resolve("dragon slain")

println("quest.getValue() =", quest.getValue())
