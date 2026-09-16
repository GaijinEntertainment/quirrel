let magazineSize = 30
let roundsFired = 7

// integer / integer truncates toward zero, it does not round
let fullReloads = magazineSize / roundsFired
println("fullReloads =", fullReloads)

// mix in a float and the result promotes to float
let secondsPerRound = 1.5
println("roundsFired * secondsPerRound =", roundsFired * secondsPerRound)

// % keeps the sign of the left operand, like C
println("-roundsFired % 3 =", -roundsFired % 3)

// DEPRECATED:
// + is the one arithmetic operator that also does string concat:
// a string operand makes + stringify and join instead of adding
println("rounds left: " + (magazineSize - roundsFired))
