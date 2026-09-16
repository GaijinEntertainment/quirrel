local built = 0

function palette() {
  built++
  return { sky = "#8ec5ff", ground = "#6b5a3e" }
}

function draw() {
  let colors = static palette()
  return colors.sky
}

println(draw())
println(draw())
println($"palette() ran {built} time(s)")

// The cached container is frozen, so no caller can corrupt it for the rest.
let colors = static palette()
try {
  colors.sky = "#000000"
} catch (e) {
  println($"write: {e}")
}

// One cache per code location, not per evaluation: the loop body caches on its
// first pass and reuses that value for every later one.
function tally() {
  let before = built
  for (local i = 0; i < 3; i++) {
    let c = static palette()
    c.sky
  }
  return built - before
}
println($"three passes through the loop called palette() {tally()} time(s)")
