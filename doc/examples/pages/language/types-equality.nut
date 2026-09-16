let squadA = { name = "alpha", strength = 4 }
let squadB = { name = "alpha", strength = 4 }
println("squadA == squadB =", squadA == squadB)   // false: tables compare by identity, not content
println("squadA == squadA =", squadA == squadA)   // true: same reference

println("1 == 1.0 =", 1 == 1.0)           // true: int and float compare by numeric value
println("null == false =", null == false)      // false: null equals only null
println("0 == false =", 0 == false)         // false: no bool/number coercion in equality either

println("1 <=> 2 =", 1 <=> 2)
println("\"alpha\" <=> \"bravo\" =", "alpha" <=> "bravo")
