function addArmor(front, side) {
  return front + side
}

// the same thing as a lambda: @(params) takes one expression and returns it
let totalArmor = @(front, side) front + side

println("addArmor(80, 40) =", addArmor(80, 40))
println("totalArmor(80, 40) =", totalArmor(80, 40))
