function reportLine() {
  return __LINE__ // the line of this statement, not of the call site
}

println("__LINE__ =", __LINE__)
println("reportLine() =", reportLine())
println("type(__FILE__) =", type(__FILE__))
