let squadRoster = { alpha = 4, bravo = 9 }

// reading and writing an existing slot works like any other variable
squadRoster.alpha += 1
println("squadRoster.alpha =", squadRoster.alpha)

// a bare name in a table literal is shorthand for name = name
let missionId = "raid07"
let briefing = { missionId, squadRoster }
println("briefing.missionId =", briefing.missionId)

// an array is written the same way, by index rather than by name
let vehiclePark = ["t34", "kv1"]
vehiclePark[0] = "is2"
println("vehiclePark[0] =", vehiclePark[0])

// <- is the only way to add a slot that does not exist yet; see the
// newslot operator on the Operators and expressions page for the full story
squadRoster.charlie <- 2
println("squadRoster.charlie =", squadRoster.charlie)
