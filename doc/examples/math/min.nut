from "math" import min

println("min(3, 1, 2) =", min(3, 1, 2))
println("min(-1.5, 2) =", min(-1.5, 2))

// the smaller value is returned as is, so its own type wins, not x's
println("type(min(5, 2.0)) =", type(min(5, 2.0)))
println("type(min(2, 5.0)) =", type(min(2, 5.0)))

try {
  min(1)
} catch (e) {
  println("min(1) throws:", e)
}
