from "test.native" import new_userdata

let ud = new_userdata()
let other = new_userdata()

function attempt(name, f) {
  try {
    println($"{name}: {f()}")
  } catch (e) {
    println($"{name}: THROW {e}")
  }
}

attempt("type", @() type(ud))
attempt("typeof", @() typeof ud)
attempt("get", @() ud.x)
attempt("set", function() { ud.x = 1 })
attempt("newslot", function() { ud.x <- 1 })
attempt("in", @() "x" in ud)
attempt("foreach", function() {
  foreach (_k, _v in ud)
    println("no slots")
})
attempt("call", @() ud())
attempt("add", @() ud + other)
attempt("unm", @() -ud)
attempt("cmp", @() ud < other)
attempt("equal to itself", @() ud == ud)
attempt("clone", @() clone ud)
attempt("tostring", @() ud.tostring())
attempt("is_frozen", @() ud.is_frozen())
attempt("getfuncinfos", @() ud.getfuncinfos())
attempt("getarity", @() ud.getarity())
