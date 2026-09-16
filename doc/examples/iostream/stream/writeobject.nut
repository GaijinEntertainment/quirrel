from "iostream" import blob

let b = blob(0)
b.writeobject([1, 2])
println("b.len() =", b.len()) // a start marker, the encoded array, an end marker

class Point { function __getstate() { return 1 } function __setstate(s) {} }
try { b.writeobject(Point()) } catch (e) { println("b.writeobject(Point()) throws:", e) } // no classes table passed

try { b.writeobject(print) } catch (e) { println("b.writeobject(print) throws:", e) } // a function cannot be serialized
