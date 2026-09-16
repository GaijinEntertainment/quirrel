let crate = { ammo = 10 }
let wr = crate.weakref()
println("wr.ref() == crate =", wr.ref() == crate)   // crate is still alive, so ref() hands it back
