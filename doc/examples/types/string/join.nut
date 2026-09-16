println("\",\".join([\"a\", \"b\", \"c\"]) =", ",".join(["a", "b", "c"]))

// bool filter: drop null and empty-string items.
println("\",\".join([\"a\", \"\", null, \"b\"], true) =", ",".join(["a", "", null, "b"], true))

// function filter: keep items the predicate accepts.
println("\",\".join([\"a\", \"bb\", \"ccc\"], filter) =", ",".join(["a", "bb", "ccc"], function(item) { return item.len() > 1 }))
