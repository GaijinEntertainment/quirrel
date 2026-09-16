from "debug" import doc

function greet() {
  @@"Prints a friendly greeting."
  println("hi")
}

println("doc(greet) =", doc(greet))
