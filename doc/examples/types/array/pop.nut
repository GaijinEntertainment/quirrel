let a = [1, 2, 3];
println("a.pop() =", a.pop());   // removes and returns the last element
println("a.len() =", a.len());
try { [].pop() } catch (e) { println("[].pop() throws:", e); }
