// _nexti hands out the next key; the VM then reads that key back through _get.
class Countdown {
  from = 0
  constructor(from) { this.from = from }

  function _nexti(previdx) {
    if (previdx == null)
      return this.from
    return previdx > 1 ? previdx - 1 : null   // null ends the loop
  }

  function _get(key) {
    if (typeof key == "integer" && key >= 1 && key <= this.from)
      return key == 1 ? "liftoff" : $"{key}..."
    throw null
  }
}

foreach (n, word in Countdown(4))
  println($"{n} {word}")
