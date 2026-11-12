// `base` is not a keyword: a method of the parent class is called by the class name
let base = 10
function scale(base) { return base * 2 }
let t = { base = 1 }

class P {
  function get() { return "P" }
}

class C(P) {
  base = 3
  function get() { return $"{P.get.call(this)}C" }
}

println($"{base} {scale(4)} {t.base} {C().base} {C().get()}")
