// forbid-root-table is checked while parsing: its effect ends with
// the block that declares it
{
  #forbid-root-table
  let armorType = "composite"
  println("armorType =", armorType)
}
::spawnPoint <- "hangar" // allowed again here
println("::spawnPoint =", ::spawnPoint)

// allow-auto-freeze is checked while generating code for the enclosing
// function: it outlives the block, unlike the directive above
{
  #allow-auto-freeze
}
let squadRoster = { name = "alpha" } // still frozen, though the block closed
println("squadRoster.is_frozen() =", squadRoster.is_frozen())
