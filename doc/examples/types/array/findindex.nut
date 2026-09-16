let a = [10, 20, 30];
println("a.findindex(v > 15) =", a.findindex(function(v) { return v > 15 }));
println("a.findindex(v > 999) =", a.findindex(function(v) { return v > 999 }));    // not found -> null
