from "string" import regexp

let digits = regexp("[0-9]+")
println("digits.match(\"12345\") =", digits.match("12345"))       // the whole string has to match
println("digits.match(\"12345 items\") =", digits.match("12345 items")) // trailing text fails the match

// a pattern that forces runaway backtracking throws rather than hanging
local s = ""
for (local i = 0; i < 26; i++) s += "a"
try {
  regexp("(a+)+b").match(s)
} catch (e) {
  println("regexp(\"(a+)+b\").match(s) throws:", e)
}
