let a = [1, 2, 3];
println("a.top() =", a.top());   // peeks the last element
println("a.len() =", a.len());    // unchanged: top() does not remove anything
try { [].top() } catch (e) { println("[].top() throws:", e); }
