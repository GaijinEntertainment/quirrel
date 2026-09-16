let a = [1, 2, 3];
println("a.hasindex(0) =", a.hasindex(0));
println("a.hasindex(2) =", a.hasindex(2));
println("a.hasindex(3) =", a.hasindex(3));
println("a.hasindex(-1) =", a.hasindex(-1));

// contrast with plain indexing, which throws instead of returning false
try { let x = a[3]; } catch (e) { println("a[3] throws:", e); }
