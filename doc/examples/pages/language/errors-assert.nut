function describeSquad(squad) {
  println("building diagnostic message")
  return $"squad {squad.name} has {squad.strength} left"
}

let squad = { name = "alpha", strength = 4 }

// a plain string message
assert(squad.strength > 0, "squad has no strength left")
println("first assert passed")

// when the message argument is a function, it only runs if the assert
// fails, so an expensive message never costs anything on the happy path
assert(squad.strength > 0, @() describeSquad(squad))
println("second assert passed, describeSquad was never called")

try {
  assert(squad.strength > 10, @() describeSquad(squad))
} catch (e) {
  println("caught:", e)
}
