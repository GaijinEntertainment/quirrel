function announce(range) {
  println($"contact at {range}m")
}

{
  let range = 400
  announce(range)
}
{
  let range = 250   // a disjoint sibling block: reusing the name is fine here
  announce(range)
}

let range = 999
function closest(range) {   // a function body is its own scope, not a nested block
  return range
}
println("closest(10) =", closest(10))
println("range =", range)
