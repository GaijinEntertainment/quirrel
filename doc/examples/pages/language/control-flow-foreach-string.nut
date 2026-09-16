let callsign = "AV8"

// one variable: each character's integer code, not a one-char string
println("callsign char codes:")
foreach (code in callsign)
  println(code)

// two variables: the position, then the same integer code
foreach (pos, code in callsign)
  println($"{pos}: {code}")
