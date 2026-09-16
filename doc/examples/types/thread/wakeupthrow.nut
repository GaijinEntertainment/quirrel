function body() {
    try { suspend() } catch(e) { println("caught", e) }
    return "recovered"
}
let t = newthread(body)
t.call()
println("t.wakeupthrow(\"boom\") =", t.wakeupthrow("boom"))          // caught inside; rethrow default is moot
println("t.getstatus() =", t.getstatus())

let t2 = newthread(body)
t2.call()
println("t2.wakeupthrow(\"boom\", false) =", t2.wakeupthrow("boom", false))  // caught inside too; rethrow only matters when uncaught
