let a = [1, 2, 3, 4];
let src = [7, 8];
let same = a.replace_with(src);
println("same == a:", same == a);     // returns this array
println("a.len() =", a.len());        // length now matches src, not the old length
println("a =", ", ".join(a.map(@(v) v.tostring())));
