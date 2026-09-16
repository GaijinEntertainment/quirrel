// only null, false, integer 0 and float 0.0 are falsy;
// "" and [] and {} are truthy, unlike some other scripting languages
println("[] =", [] ? "truthy" : "falsy")
println("0 =", 0 ? "truthy" : "falsy")

// && and || return one of their OPERANDS, not necessarily a bool,
// and short-circuit: the second side is not even evaluated
function ammoBelt() {
  println("ammoBelt() evaluated")
  return "belt_762"
}
let preferredAmmo = null
println("preferredAmmo && ammoBelt() =", preferredAmmo && ammoBelt())
println("preferredAmmo || ammoBelt() =", preferredAmmo || ammoBelt())

// ! always returns a real bool
let hitPoints = 0
println("!hitPoints =", !hitPoints)
println("typeof !hitPoints =", typeof !hitPoints)
