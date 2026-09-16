from "string" import regexp

println("regexp(\"abc\").subexpcount() =", regexp("abc").subexpcount())         // no groups: just the whole match
println("regexp(\"(?:a)(?:b)\").subexpcount() =", regexp("(?:a)(?:b)").subexpcount())  // non-capturing groups don't count
println("regexp(\"(a)(b)\").subexpcount() =", regexp("(a)(b)").subexpcount())      // two captures + the whole match

// it always equals capture(str).len() once str matches
let r = regexp("(a)(b)")
println("r.subexpcount() == r.capture(\"ab\").len() =", r.subexpcount() == r.capture("ab").len())
