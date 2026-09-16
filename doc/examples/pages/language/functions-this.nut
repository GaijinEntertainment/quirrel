let turret = {
  name = "aa_turret"
  ammo = 3
  // a function stored in a table sees the table as this
  function fire() {
    if (this.ammo <= 0)
      return $"{this.name}: empty"
    this.ammo -= 1
    return $"{this.name}: {this.ammo} left"
  }
}

println(turret.fire())
println(turret.fire())

// pulled out of the table, the same closure loses its this
let detached = turret.fire
let rebound = detached.bindenv(turret)
println(rebound())
