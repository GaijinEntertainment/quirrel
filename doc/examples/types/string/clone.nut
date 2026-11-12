// `clone` is a reserved word, so a dotted call does not even parse; reach the
// type method through a computed field access instead.
let s = "hello"
println(s["clone"]() == s)
