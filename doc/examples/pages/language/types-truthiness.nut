let checks = [
  ["null", null], ["false", false], ["0", 0], ["0.0", 0.0],
  ["empty string", ""], ["empty array", []], ["empty table", {}],
  ["nonempty string", "ready"], ["1", 1],
]
foreach (pair in checks) {
  let label = pair[0]
  let value = pair[1]
  println($"{label}: {value ? "truthy" : "falsy"}")
}
