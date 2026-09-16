let a = [10, 20, 30];
local total = 0;
let result = a.each(function(v, i) { total += v * i });
println("total =", total);      // 10*0 + 20*1 + 30*2
println("result =", result);      // each() has no return value of its own
