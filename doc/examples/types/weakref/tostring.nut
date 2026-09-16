let wr = {}.weakref()
let s = wr.tostring()
// format is "(weakref : 0x...)" - compare instead of printing the address
println("s.indexof(\"weakref\") != null =", s.indexof("weakref") != null)
println("type(s) =", type(s))
