class Rect {
  w = 0
  h = 0
  constructor(w, h) { this.w = w; this.h = h }
}
// args[0] is a required placeholder; the class supplies its own instance as "this"
let r = Rect.acall([null, 3, 4])
println("r.w =", r.w, "r.h =", r.h)
