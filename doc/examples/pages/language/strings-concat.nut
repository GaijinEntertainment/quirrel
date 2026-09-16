let weaponName = "kar98k"
let ammoLeft = 5

// prefer these to +: none of them silently stringifies a stray operand
println("".concat(weaponName, ": ", ammoLeft.tostring()))
println(", ".join(["alpha", "bravo", "charlie"]))
println("{0} has {1} rounds left".subst(weaponName, ammoLeft))

// DEPRECATED:
// + also concatenates once either side is a string - but it is still
// left-associative +, so where the string sits in the chain matters
println("1 + \"2\" =", 1 + "2")
println("2 + 3 + \"1\" =", 2 + 3 + "1")
println("\"1\" + 2 + 3 =", "1" + 2 + 3)
