let a = [1, 2, 3];
a.resize(5, 9);        // grows: new elements get the fill value
println("a =", ", ".join(a.map(@(v) v.tostring())));

a.resize(2);             // shrinks: default_value left out, extra elements are just dropped
println("a.len() =", a.len());

try { a.resize(-1) } catch (e) { println("a.resize(-1) throws:", e); }
