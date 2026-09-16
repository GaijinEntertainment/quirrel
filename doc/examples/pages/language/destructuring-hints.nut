let vehicle = { hitPoints = 250, crew = 4 }

// a field may carry a type hint, a default, or both
let { hitPoints : int, crew : int = 2, ammoBelt : array = [] } = vehicle
println($"{hitPoints} hp, crew {crew}, {ammoBelt.len()} belts")

// the hint is checked against what actually arrives
let damaged = { armor = "thick" }
try {
  let { armor : int } = damaged
  println("armor =", armor)
} catch (e) {
  println("let { armor : int } = damaged throws:", e)
}
