let a = [10, 20, 30];
println("a.findvalue(v > 15) =", a.findvalue(function(v) { return v > 15 }));
println("a.findvalue(v > 999) =", a.findvalue(function(v) { return v > 999 }));         // not found, no default
println("a.findvalue(v > 999, -1) =", a.findvalue(function(v) { return v > 999 }, -1));     // not found, default given

try { a.findvalue(function(v) { return true }, 1, 2) }
catch (e) { println("a.findvalue(..., 1, 2) throws:", e); }   // a third real argument is one too many
