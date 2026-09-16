let mission = {}

// <- adds a field that did not exist before
mission.name <- "capture_the_flag"
mission["reward"] <- 500

// once the slot exists, <- behaves exactly like a plain assignment
mission.reward <- 750
println("mission.reward =", mission.reward)

// = never creates a slot: writing an unknown field throws
try {
  mission.duration = 300
} catch (e) {
  println("caught:", e)
}
