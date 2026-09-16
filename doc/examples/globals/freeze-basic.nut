let loadout = freeze({ ammo = 10 })

println("loadout.ammo =", loadout.ammo)          // reading is normal
try { loadout.ammo = 20 } catch (e) { println("loadout.ammo = 20 throws:", e) }
