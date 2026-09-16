let a = ["a", "b", "c"];
println("a.indexof(\"b\") =", a.indexof("b"));
println("a.indexof(\"z\") =", a.indexof("z"));

let t = {};
println("[t, {}].indexof(t) =", [t, {}].indexof(t));   // reference identity, not structural equality
