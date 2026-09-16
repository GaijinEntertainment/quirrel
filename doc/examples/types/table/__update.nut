let t = {fizz=1}
let r = t.__update({buzz=2}, {fizz=10})   // later arguments win
println("r == t:", r == t)
println("t.fizz =", t.fizz, "t.buzz =", t.buzz)

try {
  t.__update()          // at least one argument is required
} catch (e) {
  println("t.__update() throws:", e)
}

try {
  t.__update("not a table")
} catch (e) {
  println("t.__update(\"not a table\") throws:", e)
}
