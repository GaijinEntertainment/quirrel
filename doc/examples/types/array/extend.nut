let a = [1];
a.extend([2, 3], [4]);   // more than one array in a single call
println("a =", ", ".join(a.map(@(v) v.tostring())));

try { [1].extend(2) } catch (e) { println("[1].extend(2) throws:", e); }        // first argument: VM type check
try { [1].extend([2], 3) } catch (e) { println("[1].extend([2], 3) throws:", e); }    // later argument: extend's own check
