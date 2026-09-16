let ammoLeft = 0

// trap: ?? binds LOOSER than comparisons, so this is ammoLeft ?? (ammoLeft > 0),
// not (ammoLeft ?? ammoLeft) > 0
println("ammoLeft ?? ammoLeft > 0 =", ammoLeft ?? ammoLeft > 0)

// trap: shifts bind TIGHTER than comparisons
println("1 << 2 == 4 =", 1 << 2 == 4)

// trap: && binds tighter than ||, so this is (squadReady && hasFuel) || override
let squadReady = true
let hasFuel = false
let override = true
println("squadReady && hasFuel || override =", squadReady && hasFuel || override)
