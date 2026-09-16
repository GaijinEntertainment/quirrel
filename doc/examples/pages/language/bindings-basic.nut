local ammoLeft = 30     // local can be reassigned
let magazineSize = 30   // let cannot: it names one value for good

ammoLeft -= 12
println("ammoLeft =", ammoLeft, "magazineSize =", magazineSize)

// magazineSize = 40  // compile error: can't assign to binding 'magazineSize'
