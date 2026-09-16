let vehicle = { turretAngle = 0.0, gunner = null }

// ?? only tests for null, unlike a truthy check: a real 0.0 survives it
println("vehicle.turretAngle ?? 45.0 =", vehicle.turretAngle ?? 45.0)

// a plain || default would get this wrong: 0.0 is falsy, so it falls
// through to 45.0 even though 0.0 is a legitimate angle
println("vehicle.turretAngle || 45.0 =", vehicle.turretAngle || 45.0)

// ?. stops a missing/null step from throwing, and once it fires the
// rest of the chain (.,  [], ()) is null-safe too, without repeating ?.
println("vehicle.gunner?.rank.tostring() =", vehicle.gunner?.rank.tostring())

// ?[ ] is the null-safe form of the index operator
let squad = null
println("squad?[\"leader\"] =", squad?["leader"])
