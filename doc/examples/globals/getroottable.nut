::GLOBAL_VAR <- 1
getroottable().GLOBAL_VAR = 2
println("::GLOBAL_VAR =", ::GLOBAL_VAR)   // a write through getroottable() reaches the real global

getroottable().NEW_ONE <- 3
println("::NEW_ONE =", ::NEW_ONE)       // new slots too

println("getroottable() == getroottable() =", getroottable() == getroottable())  // the same table every call
