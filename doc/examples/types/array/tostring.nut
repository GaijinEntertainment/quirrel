let s = [1, 2, 3].tostring();
println("type(s) =", type(s));
println("s.len() > 10:", s.len() > 10);    // the embedded address makes the string long
println("s.slice(0, 7) =", s.slice(0, 7));    // only the fixed prefix is safe to print
