from "iostream" import blob

let b = blob(3)
b[0] = 65   // 'A'
b[1] = 0    // embedded zero byte, kept as-is
b[2] = 66   // 'B'
b.seek(2)   // move the cursor away from the start

let s = b.as_string()
println("s.len() =", s.len())               // as_string ignores the cursor: all 3 bytes
println("s[0], s[1], s[2] =", $"{s[0]},{s[1]},{s[2]}")
