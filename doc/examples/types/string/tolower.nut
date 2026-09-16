println("\"ABC\".tolower() =", "ABC".tolower())
println("\"ABCDEF\".tolower(1, 3) =", "ABCDEF".tolower(1, 3))   // only the given range is converted

try { "ABCDEF".tolower(2, 100) } catch (e) println("\"ABCDEF\".tolower(2, 100) throws:", e)
try { "ABCDEF".tolower(3, 1) } catch (e) println("\"ABCDEF\".tolower(3, 1) throws:", e)
