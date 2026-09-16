let ammoCounts = { mainGun = 40, machineGun = 250 }

// two variables over a table: the key, then the value (not a sequence number)
// table order is not guaranteed, so collect the keys and sort before printing
let weaponNames = []
foreach (weaponName, count in ammoCounts)
  weaponNames.append(weaponName)
weaponNames.sort()

foreach (weaponName in weaponNames)
  println($"{weaponName}: {ammoCounts[weaponName]}")
