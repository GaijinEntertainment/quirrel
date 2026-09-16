class Animal {}
class Dog(Animal) {}
let d = Dog()
println("d.getclass() == Dog =", d.getclass() == Dog)
println("d.getclass() == Animal =", d.getclass() == Animal)
