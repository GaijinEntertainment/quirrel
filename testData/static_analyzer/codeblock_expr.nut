#allow-compiler-internals

// a block used as a statement runs for its effects, so its value is not "unused"
$${
  println("effect")
}

// 'return' and 'break' inside the block belong to the block, not to the loop
function scan(list) {
  local total = 0
  foreach (v in list) {
    total += $${
      if (v < 0)
        return 0
      return v
    }
  }
  return total
}

println(scan([1, -2, 3]))

for (local i = 0; i < 2; i++) {
  $${
    println(i)
    return null
  }
}

// a throw is not caught by the block, so it still terminates the loop
function firstOrThrow(list) {
  foreach (v in list) {
    $${
      throw $"bad {v}"
    }
  }
}

try {
  firstOrThrow([1])
} catch (e) {
  println(e)
}

// a break belongs to the block in both shapes, so the loop keeps running
function countAll(list, limit) {
  local seen = 0
  foreach (v in list) {
    $${
      if (v > limit)
        break
      try {
        if (v < 0)
          break
      } catch (e) {
      }
      return null
    }
    seen++
  }
  return seen
}

println(countAll([1, 9, -2], 5))
