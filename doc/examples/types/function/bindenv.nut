function whoami() { return this.name }
let env = {name = "Alice"}
let bound = whoami.bindenv(env)
println("bound() =", bound())                  // Alice, however bound() is called
println("{greet = bound}.greet() =", {greet = bound}.greet())  // still Alice, not the table it was read from

// the environment is held only weakly
let temp = (function() { return this }).bindenv({name = "Temporary"})
println("typeof temp() =", typeof temp()) // null: the table literal had no other owner
