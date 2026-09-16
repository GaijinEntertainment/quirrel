#forbid-clone-operator
class Loadout { ammo = 10 }
let a = Loadout()
let b = a.clone()
println("a.ammo =", a.ammo, "b.ammo =", b.ammo, "a != b =", a != b)
