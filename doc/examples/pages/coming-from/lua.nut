// indices start at 0, and an array is its own type
let squad = ["alpha", "bravo"]
println(squad[0])
println(squad.len())

// + joins as soon as one side is a string; it never reads a number out of one
println("10" + 1)
println("10".tointeger() + 1)

// a stored null keeps the slot, so removing one needs rawdelete
let ammo = { rifle = 30 }
let key = "rifle"
ammo.rifle = null
println($"after null:      len={ammo.len()} has {key}={key in ammo}")
ammo.$rawdelete(key)
println($"after rawdelete: len={ammo.len()} has {key}={key in ammo}")

// % takes its sign from the left side, as in C
println(-7 % 3)
