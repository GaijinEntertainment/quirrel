println("\"a,,b,\".split_by_chars(\",\") joined =", ",".join("a,,b,".split_by_chars(",")))     // trailing separator: no trailing empty
println("\"a,,b,\".split_by_chars(\",\", true) joined =", ",".join("a,,b,".split_by_chars(",", true))) // skip_empty drops the middle piece too
