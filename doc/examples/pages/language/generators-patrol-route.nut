// suspend() belongs to a thread of its own; a generator has none, even
// while it is running, so reaching for it here is a mistake worth catching
function patrolRoute(points) {
  foreach (point in points) {
    try {
      suspend()
    } catch (e) {
      println("wrong tool:", e)
    }
    yield point
  }
  return null
}

let patrol = patrolRoute(["gate", "tower", "bridge"])
println("resume patrol =", resume patrol)
println("resume patrol =", resume patrol)
println("resume patrol =", resume patrol)
println("patrol.getstatus() =", patrol.getstatus())
