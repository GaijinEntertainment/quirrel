local x = 10
let function foo() { return x }
let fv = foo.getfreevar(0)
println($"{fv.name}={fv.value}")
x = 99
println("foo.getfreevar(0).value =", foo.getfreevar(0).value) // reads the live variable, not a snapshot
try { foo.getfreevar(1) } catch(e) { println("foo.getfreevar(1) throws:", e) }
