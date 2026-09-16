let a = [10, 20, 30];
a.swap(0, 2);
println("a after swap(0, 2) =", ", ".join(a.map(@(v) v.tostring())));

a.swap(-1, 0);    // negative indices wrap, unlike insert() and remove()
println("a after swap(-1, 0) =", ", ".join(a.map(@(v) v.tostring())));

try { a.swap(0, 5) } catch (e) { println("a.swap(0, 5) throws:", e); }
