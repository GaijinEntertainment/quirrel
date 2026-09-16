let src = { a = 1, b = 2 }
let holder = {
  function first(x, y) {
    let one = { ...src, k1 = x }
    let two = { ...src, k2 = y }
    let three = [...[1, 2], x, y]
    let four = one.a + two.b + three.len()
    let five = four * 2 + x - y
    return { one, two, three, four, five }
  }
  function second(x, y) {
    let one = { ...src, k1 = x }
    let two = { ...src, k2 = y }
    let three = [...[1, 2], x, y]
    let four = one.a + two.b + three.len()
    let five = four * 2 + x - y
    return { one, two, three, four, five }
  }
}
println(holder.first(1, 2).four, holder.second(1, 2).four)
