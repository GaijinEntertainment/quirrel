from "string" import split_by_chars

let squadIds = split_by_chars("102-7/13", "-/")  // separators is a SET of characters

println("squadIds[0] =", squadIds[0])
println("squadIds[2] =", squadIds[2])
