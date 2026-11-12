if (__name__ == "__analysis__")
  return

const C = { b = 1 }
const ARR = [ 1 ]

function f2({ a, b = 2 }) {
  return a + b
}

function f3([ x, y ]) {
  return x + y
}

function g(src, extra) {
  f2({ b = 1 })
  f2(C)
  f3([ 1 ])
  f3(ARR)
  f2({ a = 1 })
  f3([ 1, 2 ])
  f2(src)
  f2({ ...src })
  f3([ ...extra ])
  let tbl = { b = 1 }
  f2(tbl)
}

function f4(pre, { a }) {
  return [pre, a]
}

function h() {
  f4(1, { b = 1 })
  f4({ b = 1 }, { a = 1 })
}

return [g, h]
