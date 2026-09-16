let types = require("types");

let a = types.Array(3, 0);   // same constructor that backs the global array()
println("a.len() =", a.len(), "a[0] =", a[0]);

let b = types.Array(2);       // default_value left out: every element is null
println("type(b[0]) =", type(b[0]));

try { types.Array(-1) } catch (e) { println("types.Array(-1) throws:", e); }
