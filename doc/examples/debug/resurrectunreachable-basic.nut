from "debug" import resurrectunreachable

// a reference cycle: refcounting cannot free this by itself
local squad = {}
local enemy = {}
squad.rival <- enemy
enemy.rival <- squad
squad = null
enemy = null

let found = resurrectunreachable()
println("found.len() > 0 =", found.len() > 0)
