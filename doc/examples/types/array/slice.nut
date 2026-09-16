let a = [0, 1, 2, 3, 4];
println("a.slice().len() =", a.slice().len());        // no arguments: a full copy
println("a.slice(2).len() =", a.slice(2).len());        // one argument: from start to the end
println("a.slice(-2, -1) =", ", ".join(a.slice(-2, -1).map(@(v) v.tostring())));   // negative indices count from the end
println("a.slice(1, 3, 999).len() =", a.slice(1, 3, 999).len());   // a third argument is silently ignored
