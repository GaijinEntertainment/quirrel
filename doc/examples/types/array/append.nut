let a = [1, 2];
a.append(3);           // one value
a.append(4, 5, 6);      // append() takes any number of values in a single call
println("a =", ", ".join(a.map(@(v) v.tostring())));
println("a.append(7) == a:", a.append(7) == a);   // returns this array, not a copy
