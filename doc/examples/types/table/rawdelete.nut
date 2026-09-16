let squad = {name = "alpha", strength = 4}
println("squad.$rawdelete(\"name\") =", squad.$rawdelete("name"))  // returns the removed value
println("squad.len() =", squad.len())

// an absent key is a quiet no-op, not an error
println("squad.$rawdelete(\"missing\") == null:", squad.$rawdelete("missing") == null)

// $ matters: a slot of the same name would otherwise shadow the method
let shadowed = {rawdelete = "just a field"}
println("shadowed.rawdelete =", shadowed.rawdelete)
println("type(shadowed.$rawdelete) =", type(shadowed.$rawdelete))

let frozen = freeze({name = "bravo"})
try {
  frozen.$rawdelete("name")
} catch (e) {
  println("frozen.$rawdelete(\"name\") throws:", e)
}
