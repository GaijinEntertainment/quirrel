let a = [1, 2, 3, 4];
let squares = a.map(function(v) { return v * v });
println("squares =", ", ".join(squares.map(@(v) v.tostring())));
println("squares == a:", squares == a);   // map() always returns a new array

// throwing null inside the callback drops that element instead of failing
let odds = a.map(function(v) { if (v % 2 == 0) throw null; return v });
println("odds =", ", ".join(odds.map(@(v) v.tostring())));
