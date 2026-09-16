// two functions that call each other: neither can be written second
let advanceSquad
let holdPosition

advanceSquad = function(steps) {
  return steps <= 0 ? "arrived" : holdPosition(steps - 1)
}
holdPosition = function(steps) {
  return steps <= 0 ? "dug in" : advanceSquad(steps - 1)
}

println("advanceSquad(4) =", advanceSquad(4))
println("advanceSquad(3) =", advanceSquad(3))

// reading one before its definition is a compile error, not a null
let pickTarget
pickTarget = @(squad) squad + " engaged"
println("pickTarget(alpha) =", pickTarget("alpha"))
