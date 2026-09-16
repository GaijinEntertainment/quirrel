class Animal { legs = 4 }
class Dog(Animal) {}
println("Dog.getbase() == Animal =", Dog.getbase() == Animal)
println("Animal.getbase() =", Animal.getbase())
