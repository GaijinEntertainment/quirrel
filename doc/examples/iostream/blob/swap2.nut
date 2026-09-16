from "iostream" import blob

let b = blob(5)   // odd length: 2 full pairs plus 1 leftover byte
b.seek(0)
b.writen(0x1234, 's')
b.writen(0x5678, 's')
b[4] = 0x9A

b.swap2()
println("bytes after 1st swap2() =", $"{b[0]},{b[1]},{b[2]},{b[3]}")
println("b[4] =", b[4])       // the leftover byte is never touched

b.swap2()           // swapping twice restores the original bytes
println("bytes after 2nd swap2() =", $"{b[0]},{b[1]},{b[2]},{b[3]}")
