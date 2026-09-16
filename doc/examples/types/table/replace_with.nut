let dst = {a=1, b=2}
let src = {c=3}

println("dst.replace_with(src) == dst:", dst.replace_with(src) == dst)   // the same table, mutated in place
println("dst.len() =", dst.len())                      // only src's slot remains
println("\"a\" in dst:", "a" in dst)
println("src.len() =", src.len())                      // src itself is untouched

// replacing a table with itself keeps every slot
let t = {a=1, b=2}
t.replace_with(t)
println("t.len() =", t.len())
