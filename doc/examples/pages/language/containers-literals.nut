// trailing commas are allowed after the last field
let loadout = {
  weaponName = "ak74",
  ammoBelt = 90,
}

// a key can be a string literal or a computed [expr], not just an identifier
let zone = "north"
let spawnPoints = {
  "spawn point count": 4,
  [$"spawn_{zone}"] = { x = 120, y = 40 },
}

println("loadout.weaponName =", loadout.weaponName)
println("spawn point count =", spawnPoints["spawn point count"])
println("spawnPoints.spawn_north.x =", spawnPoints.spawn_north.x)

// the comma between array elements is optional
let squadIds = [101, 102 103, 104,]
println("squadIds.len() =", squadIds.len())
