let a = [1, 2, 3];
println("a.contains(2) =", a.contains(2));
println("a.contains(9) =", a.contains(9));

// equality is by reference for tables/instances, not by contents
let t = {x = 1};
println("[t].contains(t) =", [t].contains(t));         // same table
println("[{x = 1}].contains(t) =", [{x = 1}].contains(t));   // an equal-looking but different table
