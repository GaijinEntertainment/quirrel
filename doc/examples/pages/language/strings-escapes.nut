// \n \t and friends work as in C; \xHH writes one raw byte
let briefing = "turret\tready\nstatus: \x4F\x4B"
println(briefing)

// an escape the compiler does not recognise is a compile error - a
// backslash is never just "the next character", so escape it too
println("supply\\depot")
