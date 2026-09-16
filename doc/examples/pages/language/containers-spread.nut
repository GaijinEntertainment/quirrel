let a = [1, 2]
let b = [0, ...a, 3]
println(b.len())
println(b[0], b[1], b[2], b[3])

let t = { x = 1 }
println({ ...t }.x)

// a key written after the spread wins
println({ ...t, x = 99 }.x)

// the copy is new, so writing to it leaves the source alone
let copy = { ...t }
copy.x = 5
println(t.x, copy.x)

// a frozen source gives a copy that can be written to
let locked = freeze({ y = 1 })
let unlocked = { ...locked }
unlocked.y = 2
println(unlocked.y)

// a null source adds nothing
let none = null
println([...a, ...none].len())
