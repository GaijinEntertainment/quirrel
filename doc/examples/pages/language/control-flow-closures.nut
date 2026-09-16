let turretAngles = [15, 45, 75]

// for's control variable is a single slot; every closure sees where it
// ended up, not what it held on the pass that created the closure
let staleQueue = []
for (local slot = 0; slot < turretAngles.len(); slot += 1)
  staleQueue.append(@() slot)

// a local declared inside the body is fresh each pass, so it is safe to capture
let freshQueue = []
for (local slot = 0; slot < turretAngles.len(); slot += 1) {
  let ownSlot = slot
  freshQueue.append(@() ownSlot)
}

// foreach's own loop variable is already fresh each pass, no workaround needed
let angleQueue = []
foreach (angle in turretAngles)
  angleQueue.append(@() angle)

println("staleQueue (captured for's shared slot):")
foreach (getSlot in staleQueue) print($"{getSlot()} ")
println("")
println("freshQueue (captured a fresh local per pass):")
foreach (getSlot in freshQueue) print($"{getSlot()} ")
println("")
println("angleQueue (captured foreach's own fresh variable):")
foreach (getAngle in angleQueue) print($"{getAngle()} ")
println("")
