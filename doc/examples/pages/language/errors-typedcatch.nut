class AmmoError {
  reason = ""
  constructor(reason) { this.reason = reason }
}
class NetworkError {}

function reload(ammoLeft) {
  if (ammoLeft <= 0)
    throw AmmoError("empty belt")
  return ammoLeft - 1
}

// catch clauses are tried in order; a typed one matches instanceof-style,
// the untyped one is the catch-all and must come last
try {
  reload(0)
} catch (AmmoError e) {
  println("ammo problem:", e.reason)
} catch (NetworkError e) {
  println("network problem")
} catch (e) {
  println("other:", e)
}
