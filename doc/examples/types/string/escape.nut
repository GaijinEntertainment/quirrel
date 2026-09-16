println("\"a\\\"b\\\\c\".escape() =", "a\"b\\c".escape())

// nothing to escape: the same string comes back.
println("\"plain\".escape() == \"plain\" =", "plain".escape() == "plain")
