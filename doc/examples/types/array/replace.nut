// replace is a deprecated alias; replace_with is the same function under a new name
let a = [1, 2, 3];
let same = a.replace([7, 8]);
println("same == a:", same == a);
println("a =", ", ".join(a.map(@(v) v.tostring())));
