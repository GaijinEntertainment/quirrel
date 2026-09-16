// clone is a keyword, so a literal receiver needs the operator form...
println("clone 5 =", clone 5)

// ...or bracket indexing, which calls the method without going through
// the keyword-sensitive dot grammar.
let x = 5
println("x[\"clone\"]() =", x["clone"]())
println("x[\"clone\"]() == x =", x["clone"]() == x)
