from "debug" import getlocals

function inner(x) {
  let y = 2
  let names = []
  foreach (k, v in getlocals()) names.append(k)   // default level: inner's own frame
  names.sort()
  println("getlocals() names =", ", ".join(names))

  let withThis = []
  foreach (k, v in getlocals(1, true)) withThis.append(k)
  println("getlocals(1, true) has this =", withThis.contains("this"))
}

inner(5)
println("getlocals(100).len() =", getlocals(100).len()) // no such level, but still a table
