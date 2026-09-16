enum AmmoType {
  ap = 10,
  he,               // NOT 11: an auto value counts only its own kind, from 0
  sabot,            // the second auto value, so this is 1, not 12
  designation = "smoke"
}

println("AmmoType.ap =", AmmoType.ap)
println("AmmoType.he =", AmmoType.he)
println("AmmoType.sabot =", AmmoType.sabot)
println("AmmoType.designation =", AmmoType.designation)
println("type(AmmoType.he) =", type(AmmoType.he))
println("type(AmmoType.designation) =", type(AmmoType.designation))
