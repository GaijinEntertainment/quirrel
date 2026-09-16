from "iostream" import blob

let b = blob(5)   // one full 4-byte group plus 1 leftover byte
b.seek(0)
b.writen(0x11223344, 'i')
b[4] = 0x9A

b.swap4()
println("bytes after 1st swap4() =", $"{b[0]},{b[1]},{b[2]},{b[3]}")
println("b[4] =", b[4])       // the leftover byte is never touched

b.swap4()           // swapping twice restores the original bytes
println("bytes after 2nd swap4() =", $"{b[0]},{b[1]},{b[2]},{b[3]}")
