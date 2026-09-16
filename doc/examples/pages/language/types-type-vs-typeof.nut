class Vehicle {
  function _typeof() { return "Vehicle" }
}
let tank = Vehicle()

// typeof is an operator and consults _typeof; type is a function and does not
println($"{typeof tank} vs {type(tank)}")

// the parentheses people write are around the operand, not a call
println($"{typeof(tank)} is the same as {typeof tank}")

// being a real function value, type can be stored and passed
let describe = type
println("describe([1, 2]) =", describe([1, 2]))
println("types of [1, \"alpha\", 2.5] =", ", ".join([1, "alpha", 2.5].map(type)))

// typeof cannot: `let f = typeof` does not compile, it needs an operand
