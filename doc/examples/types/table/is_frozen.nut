let t = {a=1, inner={x=1}}
println("t.is_frozen() =", t.is_frozen())

let frozen = freeze(t)
println("frozen.is_frozen() =", frozen.is_frozen())
println("frozen == t:", frozen == t)              // freeze() marks the same table, no copy
println("t.inner.is_frozen() =", t.inner.is_frozen())      // freezing is shallow: inner is untouched

t.inner.x = 2                     // still writable
println("t.inner.x =", t.inner.x)
