let a = [1, 2, 3];
let f = freeze(a);
println("a.is_frozen() =", a.is_frozen());   // freeze() froze the alias `f`, not the variable `a`
println("f.is_frozen() =", f.is_frozen());
println("a == f:", a == f);           // still the very same array underneath

a.append(4);                // mutating through the unfrozen alias works
println("f.len() =", f.len());           // f sees it too, they are one object
try { f.append(5) } catch (e) { println("f.append(5) throws:", e); }
