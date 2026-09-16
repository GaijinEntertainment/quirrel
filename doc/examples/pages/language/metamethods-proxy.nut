// A read-only view over a config table. _get and _set decide what the dot means.
class Config {
  data = null
  constructor(data) { this.data = data }

  function _get(key) {
    if (key in this.data)
      return this.data[key]
    throw null          // clean miss: not an error, just "no such slot"
  }

  function _set(key, val) {
    throw "config is read-only"
  }
}

let cfg = Config({ difficulty = "hard", lives = 3 })
println(cfg.difficulty)
println(cfg.lives)

// throw null reaches the caller as the VM's own index error
try {
  println(cfg.missing)
} catch (e) {
  println($"read: {e}")
}

// any other throw reaches the caller unchanged
try {
  cfg.lives = 99
} catch (e) {
  println($"write: {e}")
}
