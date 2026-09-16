from "math" import max

const MAGAZINE = 30
const RESERVE = MAGAZINE * 4            // arithmetic on another const
const LOADOUT = { weapon = "ak74", belts = [1, 2, 3] }
const SECOND_BELT = LOADOUT.belts[1]    // reaching into a const container
const CAP = max(MAGAZINE, 25)           // a pure function, run by the compiler
const GRADE = MAGAZINE > 10 ? "rifle" : "pistol"

println("RESERVE =", RESERVE, "SECOND_BELT =", SECOND_BELT)
println("CAP =", CAP, "GRADE =", GRADE)

// const RANDOM = rand()
// error: Only calls to pure functions are allowed in constant expressions
