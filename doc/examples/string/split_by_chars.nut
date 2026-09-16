from "string" import split_by_chars

function show(a) {
  let out = []
  foreach (s in a) out.append($"[{s}]")
  println(",".join(out))
}

// separators is a SET of characters, not a substring: any of ".-/;" splits
println("split_by_chars(\"1.2-3;;4/5\", \".-/;\") =")
show(split_by_chars("1.2-3;;4/5", ".-/;"))
println("split_by_chars(\"1.2-3;;4/5\", \".-/;\", true) =") // skip_empty drops the "" from ";;"
show(split_by_chars("1.2-3;;4/5", ".-/;", true))

// a leading separator makes an empty first piece; a trailing one makes none,
// with skip_empty or without it
println("split_by_chars(\",a,\", \",\") =")
show(split_by_chars(",a,", ","))
