let squad = { name = "alpha" }

// a slot that is not there is an error, not undefined
try {
  println(squad.hp)
} catch (e) {
  println(e)
}
println(squad?.hp ?? 100)

// two integers divide as integers, and a division by zero throws
println(7 / 2)
local zero = 0    // local: a let would fold to 0 and the analyzer would flag the division
try {
  println(1 / zero)
} catch (e) {
  println(e)
}

// for gives every closure the same variable, foreach gives each one its own
let fromFor = []
for (local i = 0; i < 3; i++)
  fromFor.append(@() i)

let fromForeach = []
foreach (v in [0, 1, 2])
  fromForeach.append(@() v)

println($"for:     {fromFor[0]()}{fromFor[1]()}{fromFor[2]()}")
println($"foreach: {fromForeach[0]()}{fromForeach[1]()}{fromForeach[2]()}")
