// _call receives the call site's own `this` first, then the arguments.
class Multiplier {
  by = 0
  constructor(by) { this.by = by }
  function _call(originalThis, x) { return x * this.by }
}

let triple = Multiplier(3)
println(triple(7))

// _cloned runs on the new object, with the original as its argument, after the
// shallow copy is already made. It is the hook for deepening that copy.
class Loadout {
  items = null
  constructor() { this.items = ["rifle"] }
  function _cloned(original) {
    this.items = clone original.items    // otherwise both share one array
  }
}

let a = Loadout()
let b = clone a
b.items.append("medkit")
println(", ".join(a.items))
println(", ".join(b.items))
