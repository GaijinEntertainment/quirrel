from "async" import Future

// A bare Future that is already settled silently ignores extra .resolve()
// calls; the terminal state is final.

async function section_after_terminal() {
  print("=== after_terminal ===\n")
  let p = Future()
  p.resolve(1)
  println($"first resolve, p={p.getState()}")
  p.resolve(2)         // no-op
  println($"after extra calls p={p.getState()}")
  let v = await p
  println($"awaited value: {v}")
  // Same for a faulted (task) future via throw-from-async.
  async function failer() { throw "first" }
  let r = failer()
  try { let _ = await r } catch (_) {}
  println($"r after fault={r.getState()}")
  try {
    let _ = await r
    print("BUG: r resolved\n")
  } catch (e) {
    println($"r reason: {e}")
  }
}

async function runAll() {
  await section_after_terminal()
  print("script done\n")
}
runAll()
