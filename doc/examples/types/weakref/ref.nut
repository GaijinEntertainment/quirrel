local t = {a = 1}
let wr = t.weakref()

// Dropping the only strong reference lets ref() see the table is gone.
t = null
println("wr.ref() =", wr.ref())
