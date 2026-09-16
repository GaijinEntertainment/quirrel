from "types" import classof, Integer, Instance

class Vehicle {
  function _typeof() { return "Vehicle" }
}
let tank = Vehicle()

println("type(tank) =", type(tank))                 // always the raw engine category
println("typeof tank =", typeof tank)                // honors a class's own _typeof
println("classof(5) == Integer =", classof(5) == Integer)      // classof(5) is the built-in Integer class
println("classof(tank) == Vehicle =", classof(tank) == Vehicle)   // classof(instance) is its own script class
println("tank instanceof Instance =", tank instanceof Instance)   // false: instanceof only walks the SCRIPT hierarchy
