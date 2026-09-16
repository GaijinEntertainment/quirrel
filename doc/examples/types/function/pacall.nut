function greet(prefix) { return $"{prefix} {this.name}" }
println("greet.pacall([{name = \"Bob\"}, \"Hi\"]) =", greet.pacall([{name = "Bob"}, "Hi"]))

function thrower() { throw "boom" }
try { thrower.pacall([{}]) } catch(e) { println("caught:", e) }
