let loadout = { weaponName = "mosin" }

let { weaponName, ammoBelt = 0 } = loadout
println($"{weaponName}: {ammoBelt} rounds")

// a missing key with no default throws instead of yielding null
try {
  let { reserveBelt } = loadout
}
catch (e) {
  println("let { reserveBelt } = loadout throws:", e)
}
