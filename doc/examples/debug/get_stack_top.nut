from "debug" import get_stack_top

println("get_stack_top() =", get_stack_top()) // always 1: just the implicit 'this'

function inner() {
  return get_stack_top() // still 1, regardless of how deep inner() is called from
}
println("inner() =", inner())

try {
  get_stack_top(42) // the signature takes no arguments
} catch (e) {
  println("get_stack_top(42) throws:", e)
}
