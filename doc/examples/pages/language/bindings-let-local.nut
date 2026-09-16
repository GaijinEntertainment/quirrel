local reloadSeconds = 4.0             // reassignable: ticks down every frame
let maxReloadSeconds = reloadSeconds  // fixed once, read back below unchanged

function tick(dt) {
  reloadSeconds -= dt
  if (reloadSeconds < 0) reloadSeconds = 0
}

tick(1.5)
tick(1.0)
println("reloadSeconds =", reloadSeconds)
println("maxReloadSeconds =", maxReloadSeconds)

// the binding is fixed, not the object it names: a let still allows this
let ammoBelt = []
ammoBelt.append("ap_round")
ammoBelt.append("he_round")
println("ammoBelt.len() =", ammoBelt.len())
