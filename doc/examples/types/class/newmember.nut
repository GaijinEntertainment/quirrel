class Foo {}
println("Foo.newmember(\"k\", \"v\") == Foo =", Foo.newmember("k", "v") == Foo)   // returns the class, so calls chain
println("Foo.k =", Foo.k)

// a third argument makes the member static: one value shared by all instances
class Bar {}
Bar.newmember("shared", 42, true)
println("Bar().shared =", Bar().shared)

// locking stops new per-instance state, but not new shared state
class Locked { x = 1 }
Locked.lock()
Locked.newmember("also_shared", 7, true)
println("Locked.also_shared =", Locked.also_shared)
try { Locked.newmember("nope", 1) } catch (e) { println("newmember(\"nope\", 1) error:", e) }
