from "string" import regexp

let r = regexp("[a-z]+")
let m = r.search("123 abc 456")
println("m.begin..m.end =", $"{m.begin}..{m.end}")
println("\"123 abc 456\".slice(m.begin, m.end) =", "123 abc 456".slice(m.begin, m.end))

// no match at or after start
println("r.search(\"ABC\", 0) =", r.search("ABC", 0))

// start also moves where ^ considers the string to begin
println("regexp(\"^b\").search(\"ab\", 1) != null =", regexp("^b").search("ab", 1) != null)
