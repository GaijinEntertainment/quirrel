let squads = [
  { name = "alpha", strength = 4 },
  { name = "bravo", strength = 9 },
  { name = "charlie", strength = 2 },
]

// @(params) body takes one EXPRESSION, and its value is the result
let ready = squads.filter(@(squad) squad.strength >= 4)
println("ready squad names:", ", ".join(ready.map(@(squad) squad.name)))

// so a brace body is a table literal, not a statement block
let makeMarker = @(squad) { name = squad.name, kind = "squad" }
let marker = makeMarker(squads[0])
println($"{type(marker)} for {marker.name}")

// which means this returns an empty table, not null
let nothing = @() {}
println("type(nothing()) =", type(nothing()))
