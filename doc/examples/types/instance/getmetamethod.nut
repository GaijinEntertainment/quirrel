class Vec {
  function _add(o) { return 1 }
}
let v = Vec()
println("v.getmetamethod(\"_add\") != null =", v.getmetamethod("_add") != null)
println("v.getmetamethod(\"_sub\") == null =", v.getmetamethod("_sub") == null)
