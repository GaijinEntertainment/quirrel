from "deep" import copy_graph

let shared = [1, 2]
let src = { a = shared, b = shared }

let graph = copy_graph(src)
println(graph.a != shared, graph.a == graph.b)

class Inventory {
  items = null
  owner = null
  constructor(items, owner) {
    this.items = items
    this.owner = owner
  }
  function _deepclone(cloner) {
    let res = clone this
    res.items = cloner(this.items)
    return res
  }
}
let inv = Inventory(["sword"], "player")
let inv2 = copy_graph(inv)
inv2.items.append("shield")
println(inv.items.len(), inv2.items.len(), inv2.owner)
