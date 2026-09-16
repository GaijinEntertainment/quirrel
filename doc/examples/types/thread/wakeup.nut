function body() {
    let r = suspend("s1")
    println("thread saw", r)
    return "done"
}
let t = newthread(body)
t.call()
println("t.wakeup(\"a\", \"b\", \"c\") =", t.wakeup("a", "b", "c")) // only "a" reaches suspend(); b, c are dropped
try { t.wakeup() } catch(e) { println("t.wakeup() throws:", e) } // already idle again
