let loadoutTemplate = { weaponName = "ak74", ammoBelt = [30, 30] }

// clone is a shallow copy: top-level slots are copied, nested
// containers (like ammoBelt here) are shared with the original
let squadLoadout = clone loadoutTemplate
squadLoadout.weaponName = "svd"
squadLoadout.ammoBelt.append(20)

println("loadoutTemplate.weaponName =", loadoutTemplate.weaponName)
println("squadLoadout.weaponName =", squadLoadout.weaponName)
println("loadoutTemplate.ammoBelt.len() =", loadoutTemplate.ammoBelt.len())
