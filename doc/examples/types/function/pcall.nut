// the calling convention is identical to call: the first argument becomes
// 'this' inside the call
function greet(prefix) { return $"{prefix} {this.name}" }
println("greet.pcall({name = \"Bob\"}, \"Hi\") =", greet.pcall({name = "Bob"}, "Hi"))

// pcall never tail-calls, unlike call: a chain of pcall recursion runs out
// of native call depth long before an equivalent call-based one would
function loopy(n) {
    if (n <= 0)
        return "done"
    return loopy.pcall(this, n - 1)
}
try { println("loopy(60) =", loopy(60)) } catch(e) { println("loopy(60) throws:", e) }
