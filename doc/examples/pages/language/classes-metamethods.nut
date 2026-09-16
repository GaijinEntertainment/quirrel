class Squad {
  squadName = "unknown"
  strength = 0
  constructor(squadName, strength) {
    this.squadName = squadName
    this.strength = strength
  }
  // array.sort() with no comparator falls back to this
  function _cmp(other) {
    return this.strength - other.strength
  }
  // println and string concatenation both call this instead of printing an address
  function _tostring() {
    return $"{this.squadName}({this.strength})"
  }
}

let squads = [Squad("alpha", 9), Squad("bravo", 4), Squad("charlie", 6)]
squads.sort()
foreach (squad in squads)
  println(squad)
