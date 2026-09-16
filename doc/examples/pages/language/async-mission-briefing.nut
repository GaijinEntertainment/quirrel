from "async" import Future

let briefingReady = Future()
briefingReady.resolve("assault the depot")

async function loadMissionBriefing() {
  println("loading briefing")
  let briefing = await briefingReady
  println("briefing:", briefing)
}

let task = loadMissionBriefing()   // returns right away; the body has not run yet
println("type(task) =", type(task))
println("continuing setup")
