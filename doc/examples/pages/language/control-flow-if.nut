function describeAmmo(beltCount) {
  // the declaration before ';' is scoped to the whole if/else chain
  if (let rounds = beltCount; rounds > 0)
    return $"{rounds} rounds left"
  else if (rounds == 0)
    return "empty"
  else
    return "invalid belt"
}

println("describeAmmo(12) =", describeAmmo(12))
println("describeAmmo(0) =", describeAmmo(0))
println("describeAmmo(-1) =", describeAmmo(-1))
