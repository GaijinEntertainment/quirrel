//expect:w213

function foo(x) { //-declared-never-used
  if (x == 1) {
    x /= 4
    ::print(x)
  }
  else {
    x /= 4
    ::print(x)
  }
}
