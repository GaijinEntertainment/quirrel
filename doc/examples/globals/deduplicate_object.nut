let t = {a = [1, 2], b = [1, 2]}   // two separate but equal arrays
println("deduplicate_object(t) =", deduplicate_object(t))       // the call itself always evaluates to null

println("t.a == t.b =", t.a == t.b)        // the equal sub-arrays were merged into one
println("getobjflags(t.a) =", getobjflags(t.a))  // the merged array is now immutable
println("getobjflags(t) =", getobjflags(t))    // the outer table itself is left as it was

try { t.a.append(3) } catch (e) { println("t.a.append(3) throws:", e) }
