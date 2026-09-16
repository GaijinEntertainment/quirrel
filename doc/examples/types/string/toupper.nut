println("\"abc\".toupper() =", "abc".toupper())
println("\"abcdef\".toupper(1, 3) =", "abcdef".toupper(1, 3))

// non-ASCII bytes are never touched.
let mb = "\xD0\x9F\xD0\xBE"
println("mb.toupper() == mb =", mb.toupper() == mb)
