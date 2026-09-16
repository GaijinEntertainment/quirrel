let types = require("types")

class Animal {
  function _typeof() { return "custom-typeof" }
}
let a = Animal()

println(types.classof(a) == Animal)           // an instance's own class
println(types.classof(Animal) == types.Class) // a class is itself of class Class
println(types.classof(5) == types.Integer)
println(type(a))  // "instance": type() ignores _typeof
println(typeof a)  // "custom-typeof": typeof calls _typeof
