class Vehicle {
  hitPoints = 100
  function describe() {
    return $"{this.hitPoints} hp"
  }
}

// (Vehicle) copies Vehicle's members first, then applies the rest of the body
class Tank(Vehicle) {
  turretAngle = 0
  constructor() {
    this.hitPoints = 250
  }
  // base reaches the overridden implementation; there is no super keyword
  function describe() {
    return base.describe() + $", turret {this.turretAngle}"
  }
}

let abrams = Tank()
println(abrams.describe())
println("abrams instanceof Vehicle =", abrams instanceof Vehicle)
