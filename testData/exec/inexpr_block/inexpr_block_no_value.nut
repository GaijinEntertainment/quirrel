#allow-compiler-internals

// a block that runs no 'return' is worth null, however it left

let noReturnAtAll = $${
  println("side effect")
}
println(typeof noReturnAtAll, noReturnAtAll)

let returnNotTaken = $${
  if (false)
    return "unreachable"
}
println(typeof returnNotTaken, returnNotTaken)

local viaBreak = "untouched"
for (local i = 0; i < 2; i++) {
  viaBreak = $${
    if (i == 1)
      break
    return "returned"
  }
  println($"i={i} {typeof viaBreak} {viaBreak}")
}

function leftByBreakInTry(v) {
  return $${
    try {
      if (v < 0)
        break
    } catch (e) {
    }
    return v * 2
  }
}

println(typeof leftByBreakInTry(-1), leftByBreakInTry(-1))
println(typeof leftByBreakInTry(3), leftByBreakInTry(3))
