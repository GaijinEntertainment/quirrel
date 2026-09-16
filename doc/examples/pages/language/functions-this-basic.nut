class Turret {
  ammo = 3
  function fire() {
    this.ammo -= 1        // this is the instance the method was called on
    return this.ammo
  }
}

let turret = Turret()
println("turret.fire() =", turret.fire())
println("turret.ammo =", turret.ammo)
