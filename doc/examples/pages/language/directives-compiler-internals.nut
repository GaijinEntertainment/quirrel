#allow-compiler-internals

// $${ ... } runs a statement block in place of an expression; its
// 'return' supplies the value. Forbidden unless allowed explicitly.
let baseHitPoints = $${
  let bonus = 20
  return 100 + bonus
}
println(baseHitPoints)
