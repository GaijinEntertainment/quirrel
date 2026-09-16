class Talkative {
  function _tostring() { return "I am talkative" }
}
println("Talkative().tostring() =", Talkative().tostring())

class Silent { x = 1 }
println("Silent().tostring().slice(0, 11) =", Silent().tostring().slice(0, 11))
