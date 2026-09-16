let a = [1, 2, 3];
println("a.remove(0) =", a.remove(0));    // removes and returns the element, not the array
println("a =", ", ".join(a.map(@(v) v.tostring())));

try { a.remove(-1) } catch (e) { println("a.remove(-1) throws:", e); }    // negative index is not wrapped
try { a.remove(a.len()) } catch (e) { println("a.remove(a.len()) throws:", e); }
