from "deep" import is_relaxed_equal

println(is_relaxed_equal({ id = 1, tags = ["a", "b"] }, { id = 1, tags = ["a", "b"] }))
println(is_relaxed_equal([1, 2], [1, 2.0]))
println(is_relaxed_equal({ id = 1 }, { id = 2 }))

class Point {
  x = 0
  y = 0
  constructor(x, y) {
    this.x = x
    this.y = y
  }
  function isEqual(o) { return o.x == this.x && o.y == this.y }
}
println(is_relaxed_equal({ at = Point(1, 2) }, { at = Point(1, 2) }))

let nearly = { float = @(a, b) (a - b) < 0.01 && (b - a) < 0.01 }
println(is_relaxed_equal([1.0], [1.005]), is_relaxed_equal([1.0], [1.005], nearly))
