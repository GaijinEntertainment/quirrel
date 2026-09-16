from "math" import rand, srand, RAND_MAX

// seed first: an unseeded rand() is not reproducible, so examples always seed
srand(1)

println("rand() =", rand())
println("rand() =", rand())
println("rand() =", rand())

// every draw stays within the documented range
let r = rand()
println("r >= 0 && r <= RAND_MAX =", r >= 0 && r <= RAND_MAX)
