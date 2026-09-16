from "debug" import doc

class Greeter {
  @@"Says hello politely."
  constructor() {}
}

println("doc(Greeter) =", doc(Greeter))
println("doc(Greeter()) =", doc(Greeter()))  // an instance looks up its class's docstring
println("doc([].insert) =", doc([].insert))  // no docstring was registered for this native
