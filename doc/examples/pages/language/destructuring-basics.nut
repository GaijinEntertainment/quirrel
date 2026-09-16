let loadout = { weaponName = "mosin", ammoBelt = 30 }
let { weaponName, ammoBelt } = loadout
println($"{weaponName}: {ammoBelt} rounds")

let spawnPoint = [120, 5, -40]
let [x, y, z] = spawnPoint
println($"spawn at ({x}, {y}, {z})")
