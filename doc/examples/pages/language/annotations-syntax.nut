function damageAt(range: number, falloff: float = 0.5): float {
  return 100.0 - range * falloff
}
println("damageAt(20) =", damageAt(20))

let describe = @(name: string, hp: int|null = null): string
  hp == null ? name : $"{name} ({hp} hp)"
println("describe(\"scout\") =", describe("scout"))
println(describe("tank", 800))

function totalAmmo(...: int): int {
  local sum = 0
  foreach (n in vargv) sum += n
  return sum
}
println("totalAmmo(30, 30, 12) =", totalAmmo(30, 30, 12))

let { weaponName: string, ammoLeft: int } = { weaponName = "mg42", ammoLeft = 120 }
println("weaponName, ammoLeft =", weaponName, ammoLeft)
