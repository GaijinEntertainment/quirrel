let squads = [
  { name = "alpha", strength = 4 },
  { name = "bravo", strength = 9 },
]

// the loop binder is a pattern, so the fields are bound directly
foreach ({ name, strength } in squads)
  println($"{name} fields {strength}")

// an index may still come first
foreach (slot, { name } in squads)
  println($"slot {slot}: {name}")

// an array element destructures by position
let spawnPoints = [[10, 20], [30, 40]]
foreach ([east, north] in spawnPoints)
  println($"spawn at {east},{north}")
