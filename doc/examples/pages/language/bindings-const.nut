const maxSquadSize = 8            // folded into every use site below, no lookup
global const gameVersion = "1.12" // also written into the shared consttable

println("maxSquadSize =", maxSquadSize)
println("gameVersion =", gameVersion)

// the fold is otherwise invisible, but its absence from consttable is not
println("getconsttable() has maxSquadSize =", "maxSquadSize" in getconsttable())
println("getconsttable() has gameVersion =", "gameVersion" in getconsttable())
