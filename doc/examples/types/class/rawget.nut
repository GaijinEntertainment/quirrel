class Animal { legs = 4 }
class Dog(Animal) { tail = true }
println("Dog.rawget(\"legs\") =", Dog.rawget("legs"))
println("Dog.rawget(\"tail\") =", Dog.rawget("tail"))
try {
  Dog.rawget("missing")
} catch (e) {
  println("Dog.rawget(\"missing\") throws:", e)
}
