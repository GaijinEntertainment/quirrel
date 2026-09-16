let mission = { squad = { name = "alpha", spawnPoint = [10, 0, 5] } }

// a pattern can only be one level deep, so unpack the outer key first...
let { squad } = mission
// ...then destructure what came out of it
let { name, spawnPoint } = squad
let [x, y, z] = spawnPoint
println($"{name} spawns at ({x}, {y}, {z})")
