from "math" import srand, rand

srand(5)
println("rand() =", rand())
println("rand() =", rand())

// re-seeding with the same value reproduces the same next draw
srand(5)
println("rand() =", rand())

// srand itself returns nothing useful
println("type(srand(5)) =", type(srand(5)))
