class Bad {
  constructor() { throw "boom" }
}
try {
  Bad.pcall(null)
} catch (e) {
  println("Bad.pcall(null) throws:", e)
}
