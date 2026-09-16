let loadout = { rifle = "ak74", helmet = "ssh68" }
println("\"rifle\" in loadout =", "rifle" in loadout)
println("\"boots\" not in loadout =", "boots" not in loadout)

// on an array 'in' tests the INDEX, not the value: it asks
// "is there a slot at this position", not "does this value occur"
let ammoBelt = [30, 30, 20]
println(1 in ammoBelt)     // index 1 exists
println(20 in ammoBelt)    // 20 is a value in the array, but not a valid index

class Vehicle {}
class Tank(Vehicle) {}
let myTank = Tank()
println("myTank instanceof Vehicle =", myTank instanceof Vehicle)
println("myTank instanceof Tank =", myTank instanceof Tank)

println("typeof myTank =", typeof myTank)
println("typeof ammoBelt =", typeof ammoBelt)
