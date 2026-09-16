println("getconsttable().SQOBJ_FLAG_IMMUTABLE =", getconsttable().SQOBJ_FLAG_IMMUTABLE)  // an engine-defined constant

getconsttable().MY_CONST <- 123  // the table itself is an ordinary mutable table
println("getconsttable().MY_CONST =", getconsttable().MY_CONST)

println("getconsttable() == getconsttable() =", getconsttable() == getconsttable())  // the same table every call
