if (__name__ == "__analysis__")
  return

const C = { x = 1 }

function f2({ a }) {
  return a
}

function f3([ x ]) {
  return x
}

function g(src) {
  f2([ 1 ])
  f3({ x = 1 })
  f3(C)
  let tbl = { x = 1 }
  f3(tbl)
  local loc = { x = 1 }
  f3(loc)
  f3(src)
  f2({ a = 1 })
}

return g
