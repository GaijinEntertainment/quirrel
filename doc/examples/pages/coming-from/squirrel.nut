// filter passes (value, index), which is the other way round
let heavy = [10, 20, 30].filter(@(value, index) value > 15)
println(heavy.len())

// find is indexof, and a miss answers null instead of -1
println([10, 20].indexof(20))
println([10, 20].indexof(99))

// a method reaches its own class through this
class Squad {
  size = 2
  function report() { return this.describe() }
  function describe() { return $"size {this.size}" }
}
println(Squad().report())

// delete is off; rawdelete removes a slot
let ammo = { rifle = 30 }
ammo.$rawdelete("rifle")
println(ammo.len())
