const SPREAD_SOURCE = { a = 1 }
let viaSpread = static { ...SPREAD_SOURCE, k = 1 }
let plain = static { a = 1, k = 1 }
println(viaSpread.a, plain.a)
