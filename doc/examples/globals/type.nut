println("type(5) =", type(5))
println("type(\"s\") =", type("s"))
println("type([1]) =", type([1]))
println("type(println) =", type(println))  // a native closure is still "function"

class Custom {
  function _typeof() { return "custom" }  // the `typeof` operator honors this
}
println("type(Custom()) =", type(Custom()))   // type() ignores it: always the raw engine type
println("typeof Custom() =", typeof Custom())  // typeof does not
