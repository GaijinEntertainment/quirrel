let squads = [
  { name = "bravo", strength = 9 },
  { name = "alpha", strength = 4 },
  { name = "charlie", strength = 6 },
]

// == != < <= > >= all return a real bool
println("squads[0].strength > squads[1].strength =", squads[0].strength > squads[1].strength)

// <=> returns an int: negative, zero or positive, not just a bool,
// which is exactly what sort() wants from its comparator
squads.sort(@(a, b) a.strength <=> b.strength)
foreach (squad in squads)
  println($"{squad.name}: {squad.strength}")

// the ternary picks one of two expressions, not two statements
let weakest = squads[0]
println("weakest.strength > 5 =", weakest.strength > 5 ? "combat ready" : "needs reinforcement")
