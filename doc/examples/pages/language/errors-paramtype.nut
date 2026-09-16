function heal(target, hp: int) {
  target.hp += hp
  return target.hp
}

let medic = { hp = 50 }

// this is the message people hit most: a type-annotated parameter
// (or a native function's own argument check) got the wrong type
try {
  heal(medic, "ten")
} catch (e) {
  println("caught:", e)
}

println("heal(medic, 10) =", heal(medic, 10))
