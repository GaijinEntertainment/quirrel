local totalDamage = 0
for (local shot = 0; shot < 5; shot += 1)
  totalDamage += 10 + shot
println($"total damage: {totalDamage}")

// several variables and several increments, comma-separated
for (local front = 0, back = 4; front < back; front += 1, back -= 1)
  println($"swap {front} <-> {back}")
