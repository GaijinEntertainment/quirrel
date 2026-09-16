println("reduce([1, 2, 3]), no initial =", [1, 2, 3].reduce(function(a, b) { return a + b }));
println("reduce([1, 2, 3]), initial 10 =", [1, 2, 3].reduce(function(a, b) { return a + b }, 10));
println("reduce([7]), no initial =", [7].reduce(function(a, b) { return a + b }));                // one element: callback never runs
println("reduce([]), no initial =", [].reduce(function(a, b) { return a + b }));
println("reduce([]), initial 42 =", [].reduce(function(a, b) { return a + b }, 42));
