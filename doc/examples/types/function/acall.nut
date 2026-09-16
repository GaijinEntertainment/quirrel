function greet(prefix) { return $"{prefix} {this.name}" }
println("greet.acall([{name = \"Bob\"}, \"Hi\"]) =", greet.acall([{name = "Bob"}, "Hi"]))

// acall takes exactly one array argument - it is not itself variadic
try { greet.acall([{name = "Bob"}, "Hi"], "extra") } catch(e) { println("greet.acall(..., \"extra\") throws:", e) }
try { greet.acall(1) } catch(e) { println("greet.acall(1) throws:", e) }
