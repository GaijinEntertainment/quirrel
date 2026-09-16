let turretConfig = { turnSpeed = 45, elevation = 20 }
let sharedRef = turretConfig       // alias made before freeze
let locked = freeze(turretConfig)  // a new, immutable reference to the same table

try { locked.turnSpeed = 90 } catch (e) { println("locked.turnSpeed = 90 throws:", e) }
sharedRef.turnSpeed = 90             // still allowed: sharedRef was never frozen
println("locked.turnSpeed =", locked.turnSpeed) // visible here too: they name the same table
