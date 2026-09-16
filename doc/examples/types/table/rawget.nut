let t = {a=1, b=2}
println("t.rawget(\"a\") =", t.rawget("a"))
println("t[\"a\"] =", t["a"])          // same result: a table has no reachable _get here

try {
  t.rawget("missing")
} catch (e) {
  println("t.rawget(\"missing\") throws:", e)
}

try {
  t.missing               // plain indexing also throws, with different wording
} catch (e) {
  println("t.missing throws:", e)
}
