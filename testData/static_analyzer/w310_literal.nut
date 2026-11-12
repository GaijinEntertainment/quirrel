if (__name__ == "__analysis__")
  return

const C = { a = 1 }
let outer = { o = 1 }
local outerLocal = {}

function reset() { outerLocal = [] }

function f(src) {
  let [ x, y ] = { x = 1 }
  let { p } = [ 1 ]
  let [ a ] = C
  let tbl = { m = 1 }
  let [ m ] = tbl
  let [ o ] = outer
  local arr = [ 1 ]
  let { n } = arr
  local re = {}
  let set = function() { re = [ 1 ] }
  set()
  let [ r ] = re
  let [ ol ] = outerLocal
  let { def = {} } = src
  let [ dv ] = def
  let alias = tbl
  let [ al ] = alias
  return [x, y, p, a, m, o, n, r, ol, dv, al]
}

return [f, reset]
