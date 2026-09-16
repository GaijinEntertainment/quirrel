let a = [3, 1, 2];
a.sort();                          // no comparator: default ascending order
println("a sorted =", ", ".join(a.map(@(v) v.tostring())));

let b = [3, 1, 2];
b.sort(function(x, y) { return y <=> x });   // one comparator: descending order
println("b sorted desc =", ", ".join(b.map(@(v) v.tostring())));

// two extra arguments: the comparator is silently ignored, default order wins
let c = [3, 1, 2];
c.sort(function(x, y) { return y <=> x }, "ignored");
println("c sorted (comparator ignored) =", ", ".join(c.map(@(v) v.tostring())));
