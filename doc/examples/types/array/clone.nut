// clone is a language keyword, so a.clone() does not even parse
try { compilestring("return [].clone()") } catch (e) { println("[].clone() throws:", e); }

let a = [1, [2, 3]];
let b = clone a;      // use the operator instead
b[0] = 99;
b[1].append(4);          // the nested array is shared: this is a shallow copy
println("a[0] =", a[0], "b[0] =", b[0]);
println("a[1].len() =", a[1].len());     // grew through b, because a[1] and b[1] are the same array
