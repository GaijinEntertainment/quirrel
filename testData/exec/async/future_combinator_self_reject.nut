from "async" import Future

// An input that faults with the combinator's own result future must not wedge the
// result pending: done.reject(done) would hit the self-settle guard and throw, so
// the combinator substitutes a diagnostic fault and still settles the result.

async function check(label, f) {
    try { let _ = await f; println($"{label}: BUG fulfilled") }
    catch (e) { println($"{label}: {e}") }
}

let p1 = Future()
let r1 = Future.race([p1])
p1.reject(r1)
check("race", r1)

let p2 = Future()
let r2 = Future.all([p2])
p2.reject(r2)
check("all", r2)

print("script done\n")
