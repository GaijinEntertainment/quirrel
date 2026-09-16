let a = [1, 2, 3, 4, 5];
let evens = a.filter(function(v) { return v % 2 == 0 });
println("evens =", ", ".join(evens.map(@(v) v.tostring())));
println("evens == a:", evens == a);   // filter() always returns a new array
