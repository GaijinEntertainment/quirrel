class Weapon {
  weaponName = "unknown"
  ammoBelt = 0
  constructor(weaponName, ammoBelt) {
    this.weaponName = weaponName
    this.ammoBelt = ammoBelt
  }
  function fire() {
    if (this.ammoBelt <= 0)
      return $"{this.weaponName}: empty"
    this.ammoBelt -= 1
    return $"{this.weaponName}: {this.ammoBelt} left"
  }
}

let rifle = Weapon("ak74", 2)
println(rifle.fire())
println(rifle.fire())
println("rifle instanceof Weapon =", rifle instanceof Weapon)
