class Rect {
  w = 0
  h = 0
  constructor(w, h) { this.w = w; this.h = h }
}
// the leading argument is a required placeholder, discarded in favor of the new instance
let r = Rect.call(null, 5, 6)
println("r.w =", r.w, "r.h =", r.h)
