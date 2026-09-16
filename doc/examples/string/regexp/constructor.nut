from "string" import regexp

let r = regexp("[a-zA-Z]+[0-9]*")
println("r.match(\"item42\") =", r.match("item42"))
println("r.match(\"42item\") =", r.match("42item"))

// the same compiled pattern is reused across calls
println("r.match(\"abc\") =", r.match("abc"))

try {
  regexp("[a-z") // unterminated character class
} catch (e) {
  println("regexp(\"[a-z\") throws:", e)
}
