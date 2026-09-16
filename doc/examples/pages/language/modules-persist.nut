let missionStats = persist("missionStats", @() { completed = 0 })
missionStats.completed += 1
println("missionStats.completed =", missionStats.completed)

let squadRoster = keepref(["scout", "sniper", "medic"])
println("squadRoster.len() =", squadRoster.len())

println("__name__ =", __name__)
