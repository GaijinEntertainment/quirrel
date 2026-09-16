from "string" import regexp

let caps = regexp("(\\d+)-(\\d+)").capture("id 12-34 end")
println("caps.len() =", caps.len())                      // whole match + 2 groups
foreach (i, c in caps)
  println($"{i}: {c.begin}..{c.end}")

// a group inside an alternative that did not run reports an empty span
// at the very start of the string, not null
let alt = regexp("(a)|(b)").capture("xb")
println("alt[1].begin..alt[1].end =", $"{alt[1].begin}..{alt[1].end}")
