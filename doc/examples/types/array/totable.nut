let t1 = [["a", 1], ["b", 2]].totable();   // array of [key, value] pairs
println("t1.a =", t1.a, "t1.b =", t1.b);

let t2 = ["x", "y"].totable();               // array of simple values: value becomes the key too
println("t2.x =", t2.x, "t2.y =", t2.y);

try { [["a", 1], ["b", 2, 3]].totable() } catch (e) { println("pair with 3 elements throws:", e); }
try { [1, [2]].totable() } catch (e) { println("non-pair element throws:", e); }
