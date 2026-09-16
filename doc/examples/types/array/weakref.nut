let a = [1, 2, 3];
let w = a.weakref();
println("type(w) =", type(w));
println("w.ref() == a:", w.ref() == a);   // ref() gets the original array back
println("w.ref().len() =", w.ref().len());
