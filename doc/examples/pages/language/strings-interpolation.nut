let squadName = "alpha"
let hitPoints = 87

// {expr} substitutes; the whole literal compiles to a call to subst
println($"{squadName}: {hitPoints} hp")

// a brace meant to print literally has to be escaped
println($"damage taken: \{{hitPoints}\}")

// a hole may hold another interpolated string
println($"squad: {$"[{squadName}]"}")
