// // starts a comment, so a floor division quietly loses its right side
let half = 7 // 2
println(half)

// two integers divide as integers, and neither / nor % is floored
println(7 / 2)
println(-7 / 2)
println(-7 % 3)

// only null, false, 0 and 0.0 are false
println([] ? "[] is true" : "[] is false")
println("" ? "empty string is true" : "empty string is false")

// in asks about a key, and the keys of an array are its indices
println(2 in [10, 20, 30])
println(99 in [10, 20, 30])
println([10, 20, 30].contains(20))
