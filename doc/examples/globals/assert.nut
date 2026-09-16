try { assert(1 + 1 == 3, "math is broken") } catch (e) { println("assert(1+1==3, ...) throws:", e) }

// the message may be a function; it runs only when the assertion actually fails
local pings = 0
function build_message() { pings += 1; return "built on demand" }

assert(true, build_message)
println("pings =", pings)                              // still 0: condition held, message unused

try { assert(false, build_message) } catch (e) { println("assert(false, ...) throws:", e) }
println("pings =", pings)                               // now 1: message was built to form the error
