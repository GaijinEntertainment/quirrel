function body(a, b) {
    println("a, b =", a, b)
    let r = suspend("first")
    println("resumed with", r)
    return "done"
}
let t = newthread(body)
println("t.call(\"hello\", \"world\") =", t.call("hello", "world"))
println("t.wakeup(\"go\") =", t.wakeup("go"))
