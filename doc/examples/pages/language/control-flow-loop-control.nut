let contacts = [
  { name = "wreck", hp = 0, hostile = false },
  { name = "scout", hp = 40, hostile = false },
  { name = "tank", hp = 100, hostile = true },
]

function firstArmedContact(list) {
  foreach (contact in list) {
    if (contact.hp <= 0)
      continue            // skip wrecks, keep scanning
    if (contact.hostile)
      return contact.name // leaves the function, not just the loop
  }
  return null
}
println("first armed contact:", firstArmedContact(contacts))

local scanned = 0
foreach (contact in contacts) {
  if (contact.hostile)
    break                  // leaves only the loop
  scanned += 1
}
println($"scanned before first threat: {scanned}")
