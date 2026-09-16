from "math" import min

let units = [
  { name = "scout", hp = 40, ammo = 8 },
  { name = "tank", hp = 120, ammo = 0 },
  { name = "drone", hp = 0, ammo = 12 },
]

function nextAction(hp, ammo) {
  if (hp <= 0)
    return "respawn"

  if (ammo == 0)
    return "reload"

  let burst = min(ammo, 3)
  return $"attack ({burst} shots)"
}

foreach ({name, hp, ammo} in units)
  println($"{name}: {nextAction(hp, ammo)}")
