// damage falls off with distance; the falloff rate has a default
function computeDamage(baseDamage, distance, falloffPerMeter = 0.5) {
  let reduced = baseDamage - distance * falloffPerMeter
  return reduced > 0 ? reduced : 0
}

println("computeDamage(100, 40) =", computeDamage(100, 40))
println("computeDamage(100, 40, 2.0) =", computeDamage(100, 40, 2.0))

// a trailing ... collects the rest of the arguments into vargv
function describeLoadout(vehicleName, ...) {
  println($"{vehicleName} carries {vargv.len()} modules: {", ".join(vargv)}")
}

describeLoadout("t34_tank")
describeLoadout("t34_tank", "smoke_launcher", "spare_tracks")
