class Foo { x = 1 }
Foo.lock()

try { Foo.rawset("z", 1) } catch (e) { println("rawset(\"z\", 1) error:", e) }

// locking stops new per-instance state; a static member is still allowed
Foo.newmember("shared", 42, true)
println("Foo.shared =", Foo.shared)
