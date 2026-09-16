let a = [1, 2, 3];
a.insert(1, 99);
println("a =", ", ".join(a.map(@(v) v.tostring())));

a.insert(a.len(), 100);   // inserting at len() behaves like append
println("a.top() =", a.top());

try { a.insert(-1, 0) } catch (e) { println("a.insert(-1, 0) throws:", e); }    // negative index is not wrapped
try { a.insert(999, 0) } catch (e) { println("a.insert(999, 0) throws:", e); }
