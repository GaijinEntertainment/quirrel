function greet(prefix) { return $"{prefix} {this.name}" }
let bob = {name = "Bob"}
println("greet.call(bob, \"Hi\") =", greet.call(bob, "Hi"))

// calling a generator-producing function through call() just creates the
// generator, exactly like calling it directly
function geny(n) { yield n }
let g = geny.call({}, 3)
println("typeof g + \" \" + resume g =", typeof g, resume g)
