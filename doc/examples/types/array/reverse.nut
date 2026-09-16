let a = [1, 2, 3];
let same = a.reverse();
println("same == a:", same == a);   // returns this array, reversed in place
println("a =", ", ".join(a.map(@(v) v.tostring())));
