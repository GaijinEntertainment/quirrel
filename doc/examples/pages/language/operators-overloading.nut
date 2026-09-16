class MyPoint2 {
  x = 0
  y = 0
  constructor(ax, ay) { this.x = ax; this.y = ay }

  // getclass() rather than the class name: the name is not in scope inside the
  // body, and this keeps working for a subclass
  function _add(other) { return this.getclass()(this.x + other.x, this.y + other.y) }
  function _sub(other) { return this.getclass()(this.x - other.x, this.y - other.y) }
  function _mul(k)     { return this.getclass()(this.x * k, this.y * k) }
  function _unm()      { return this.getclass()(-this.x, -this.y) }
  // one _cmp drives <, <=, >, >= and == at once
  function _cmp(other) { return (this.x * this.x + this.y * this.y)
                             <=> (other.x * other.x + other.y * other.y) }
  function _tostring() { return $"({this.x}, {this.y})" }
  function _typeof()   { return "MyPoint2" }
}

let muzzle = MyPoint2(3, 4)
let recoil = MyPoint2(1, 2)

println("muzzle + recoil =", muzzle + recoil)
println("muzzle - recoil =", muzzle - recoil)
println("muzzle * 2 =", muzzle * 2)
println("-muzzle =", -muzzle)
println("muzzle > recoil =", muzzle > recoil)
println("muzzle == recoil =", muzzle == recoil)
println("typeof muzzle =", typeof muzzle)
println("type(muzzle) =", type(muzzle))
