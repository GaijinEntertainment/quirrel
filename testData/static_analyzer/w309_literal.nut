if (__name__ == "__analysis__")
  return

const C = { a = 1 }
const ARR = [ 1 ]

function f(k, src, extra) {
  let { x, y } = { x = 1 }
  let { a, b } = C
  let [ p, q ] = [ 1 ]
  let [ e, g ] = ARR
  let { d = 2 } = { x = 1 }
  let [ h, i = 3 ] = [ 1 ]
  let { s, t } = { [k] = 1 }
  let { u, w } = { ...src }
  let [ j, l ] = [ ...extra ]
  let tbl = { m = 1 }
  tbl.n <- 2
  let { m, n } = tbl
  return [x, y, a, b, p, q, e, g, d, h, i, s, t, u, w, j, l, m, n]
}

return f
