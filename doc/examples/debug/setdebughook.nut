from "debug" import setdebughook

let counts = {}
setdebughook(function(kind, src, line, name) {
  if (kind in counts) counts[kind] = counts[kind] + 1
  else counts[kind] <- 1
})

// a fresh thread call gives the hook a call boundary that starts after it was
// installed; this whole top-level script, already running, would not be seen
let co = newthread(function(a, b) { return a + b })
co.call(1, 2)

setdebughook(null)
println("calls:", counts['c'])
println("returns:", counts['r'])
