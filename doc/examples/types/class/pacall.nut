class Bad {
  constructor() { throw "boom" }
}
try {
  Bad.pacall([null])
} catch (e) {
  println("Bad.pacall([null]) throws:", e)
}
