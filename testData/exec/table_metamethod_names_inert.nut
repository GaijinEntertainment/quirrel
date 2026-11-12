let t = {
  _get = @(k) $"get {k}"
  _set = @(_k, _v) null
  _newslot = @(_k, _v) null
  _delslot = @(_k) null
  _add = @(_o) "add"
  _sub = @(_o) "sub"
  _mul = @(_o) "mul"
  _div = @(_o) "div"
  _modulo = @(_o) "mod"
  _unm = @() "unm"
  _cmp = @(_o) 0
  _typeof = @() "custom"
  _call = @(_e, x) x
  _cloned = @(_o) null
  _nexti = @(_i) null
  _tostring = @() "custom string"
  _lock = @() null
}
let other = { x = 1 }

function attempt(name, f) {
  try {
    println($"{name}: {f()}")
  } catch (e) {
    println($"{name}: THROW {e}")
  }
}

attempt("get", @() t.missing)
attempt("set", function() { t.missing = 1 })
attempt("newslot", function() { t.fresh <- 1; return t.$rawget("fresh") })
attempt("add", @() t + other)
attempt("sub", @() t - other)
attempt("mul", @() t * other)
attempt("div", @() t / other)
attempt("modulo", @() t % other)
attempt("unm", @() -t)
attempt("cmp", @() t < other)
attempt("typeof", @() typeof t)
attempt("call", @() t(5))
attempt("tostring", @() t.tostring().slice(0, 6))
attempt("interpolation", @() $"{t}".slice(0, 6))
attempt("foreach", function() {
  local n = 0
  foreach (_k, _v in t)
    n++
  return n
})
attempt("clone", @() (clone t).len())
attempt("getfuncinfos", @() t.getfuncinfos())
attempt("getarity", @() t.getarity())
attempt("setdelegate", @() t.setdelegate({}))
attempt("getdelegate", @() t.getdelegate())
