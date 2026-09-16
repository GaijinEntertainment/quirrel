let math = require("math")

// this method and math.hash() do not agree on the number itself.
println("\"abc\".hash() == math.hash(\"abc\") =", "abc".hash() == math.hash("abc"))
