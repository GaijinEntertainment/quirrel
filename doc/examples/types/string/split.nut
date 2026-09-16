println("\" a  b\\tc \".split() joined =", ",".join(" a  b\tc ".split()))     // no separator: split on white space runs

// a literal substring separator, unlike split_by_chars's set of bytes.
println("\"a::b::c\".split(\"::\") joined =", ",".join("a::b::c".split("::")))

// a trailing separator DOES make a trailing empty piece here.
println("\"a,b,\".split(\",\").len() =", "a,b,".split(",").len())
