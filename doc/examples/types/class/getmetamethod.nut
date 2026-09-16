class Vec {
  function _add(o) { return 1 }
}
println("Vec.getmetamethod(\"_add\") != null =", Vec.getmetamethod("_add") != null)
println("Vec.getmetamethod(\"_sub\") == null =", Vec.getmetamethod("_sub") == null)
try {
  Vec.getmetamethod("_bogus")
} catch (e) {
  println("Vec.getmetamethod(\"_bogus\") throws:", e)
}
