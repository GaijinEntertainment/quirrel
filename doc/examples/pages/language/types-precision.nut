from "debug" import getbuildinfo

let shellCount = 16777217           // 2^24 + 1
let asFloat = shellCount.tofloat()  // single-precision float cannot hold it exactly
println("asFloat.tointeger() =", asFloat.tointeger())          // rounded down to 2^24

// comparing an int to a float promotes the int through the same lossy cast
println("shellCount == asFloat =", shellCount == asFloat)

println("getbuildinfo().floatsize =", getbuildinfo().floatsize)     // 4: this build's float is single precision
