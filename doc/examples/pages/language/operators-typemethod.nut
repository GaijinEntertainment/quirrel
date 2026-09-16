let squad = { name = "alpha" }
println("squad.$len() =", squad.$len())      // the built-in table method

// a slot can carry the same name as a method, and then it wins
let shadowed = { name = "bravo", len = "a field, not a method" }
println("shadowed.len =", shadowed.len)
println("shadowed.$len() =", shadowed.$len())  // $ never sees the slot

try { shadowed.len() } catch (e) { println("shadowed.len() throws:", e) }
