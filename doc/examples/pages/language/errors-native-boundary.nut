// array.sort() is a native function that calls back into this comparator;
// a throw inside it crosses that native frame and reaches an ordinary
// try/catch around the call, exactly as if no native code were involved
let squads = [{ name = "alpha", hp = 5 }, { name = "bravo", hp = null }]

try {
  squads.sort(function(a, b) {
    if (a.hp == null || b.hp == null)
      throw "squad with unknown hp"
    return a.hp <=> b.hp
  })
} catch (e) {
  println("caught through sort():", e)
}
