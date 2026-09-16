let squadMembers = ["driver", "gunner", "loader"]

// one variable: the value
println("squadMembers roles:")
foreach (role in squadMembers)
  println(role)

// two variables: the position, then the value
foreach (seat, role in squadMembers)
  println($"seat {seat}: {role}")
