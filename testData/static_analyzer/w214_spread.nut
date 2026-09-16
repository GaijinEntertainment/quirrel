let t = { a = 1 }
let c = true
let x = c ? { ...t } : { ...t }
println(x.a)
