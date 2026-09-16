from "iostream" import blob

let b = blob(0)
b.writeobject(42)
b.seek(0)
println("b.readobject() =", b.readobject()) // 42

let empty = blob(0)
try { empty.readobject() } catch (e) { println("empty.readobject() throws:", e) } // nothing was ever written

class Point { function __getstate() { return 1 } function __setstate(s) {} }
let b2 = blob(0)
b2.writeobject(Point(), {Point=Point})
b2.seek(0)
try { b2.readobject() } catch (e) { println("b2.readobject() throws:", e) } // classes omitted this time
