// A loader that reports progress. The nested call is the point: a generator
// could not suspend from in there, a thread can.
function readChunk(name) {
  let ack = suspend($"loading {name}")
  return $"{name} ({ack})"
}

function loadAll(first, second) {
  let a = readChunk(first)
  let b = readChunk(second)
  return $"done: {a}, {b}"
}

let loader = newthread(loadAll)
println(loader.getstatus())

// call's arguments are the thread function's parameters, and it returns
// whatever the first suspend handed over
println(loader.call("terrain", "props"))
println(loader.getstatus())

// wakeup's argument becomes the return value of that suspend
println(loader.wakeup("ok"))
println(loader.wakeup("ok"))
println(loader.getstatus())
