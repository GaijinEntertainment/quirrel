let squad = { strength = 4 }
squad.strength += 3
squad.strength *= 2
println("squad.strength =", squad.strength)

local reloadSeconds = 3
// post: yields the old value, THEN steps; pre: steps first, yields the new one
println("reloadSeconds-- =", reloadSeconds--)
println("--reloadSeconds =", --reloadSeconds)

// compound assignment still requires the slot to already exist
try {
  squad.readiness += 1
} catch (e) {
  println("caught:", e)
}
