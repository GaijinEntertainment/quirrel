from "async" import Future

// External .resolve() on a task-future (the future returned by an async
// function) must throw regardless of task lifecycle. Task identity is
// latched at construction, not derived from generator presence. Three phases:
//   - inflight: task has not finished its body
//   - returns_future: task body returned a Future (stored verbatim, no adoption)
//   - after_done: task body fully complete
// Bare Futures remain unaffected.

async function section_inflight() {
  print("=== inflight ===\n")
  async function makeTask() { return 42 }
  let task = makeTask()
  try {
    task.resolve(99)
    print("BUG: resolve no throw\n")
  } catch (e) {
    println($"resolve threw: {e}")
  }
  let bare = Future()
  bare.resolve("ok")
  println($"bare: {bare.getState()}")
  let r = await task
  println($"consumer got: {r}")
}

async function section_returns_future() {
  print("=== returns_future ===\n")
  let inner = Future()
  async function f() { return inner }   // body returns a Future; no adoption, fulfils with it verbatim
  let task = f()
  // One suspension so f's body completes and the task-future fulfils.
  let tick = Future()
  tick.resolve(null)
  let _ = await tick
  try {
    task.resolve(99)
    print("BUG: resolve no longer throws\n")
  } catch (e) {
    println($"resolve still throws: {e}")
  }
  inner.resolve("real")
  // No adoption: `await task` yields the inner Future (one level); await again for the value.
  let innerFut = await task
  let r = await innerFut
  println($"consumer got: {r}")
}

async function section_after_done() {
  print("=== after_done ===\n")
  async function f() { return 42 }
  let task = f()
  let r = await task
  try {
    task.resolve(99)
    print("BUG: resolve no longer throws\n")
  } catch (e) {
    println($"resolve throws: {e}")
  }
  let r2 = await task
  println($"after probe, task awaits to: {r2}")
}

async function runAll() {
  await section_inflight()
  await section_returns_future()
  await section_after_done()
  print("script done\n")
}
runAll()
