from "deep" import copy

let shared = [1, 2]
let src = { a = shared, b = shared }

let plain = copy(src)
println(plain.a != shared, plain.a == plain.b)

class Inventory {
  items = null
}
let inv = Inventory()
println(copy(inv) == inv)
