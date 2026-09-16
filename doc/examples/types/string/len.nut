println("\"hello\".len() =", "hello".len())

// a Quirrel string is a byte buffer: this 2-byte UTF-8 character counts as 2.
let mb = "\xD0\x9F"
println("mb.len() =", mb.len())

try { "hello".len(1) } catch (e) println("\"hello\".len(1) throws:", e)
