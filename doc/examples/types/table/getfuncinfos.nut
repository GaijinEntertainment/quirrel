let t = {a=1}
// a plain slot named "_call" is not a metamethod: getfuncinfos only looks
// at the table's delegate, and script code has no way to attach one
t._call <- function(self, x) { return x }
println("t.getfuncinfos() =", t.getfuncinfos())
