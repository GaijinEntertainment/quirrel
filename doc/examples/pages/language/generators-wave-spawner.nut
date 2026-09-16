// each resume advances the spawner to the next wave; the generator is
// suspended, never running, until the first resume touches it
function waveSpawner(waveCount) {
  for (local waveNumber = 1; waveNumber <= waveCount; waveNumber++)
    yield waveNumber * 3   // enemies to spawn this wave
  return null
}

let spawner = waveSpawner(3)
println("spawner.getstatus() =", spawner.getstatus())

local enemyCount = resume spawner
while (enemyCount) {
  println($"spawn {enemyCount} enemies")
  enemyCount = resume spawner
}
println("spawner.getstatus() =", spawner.getstatus())

try {
  resume spawner
} catch (e) {
  println("resume spawner throws:", e)
}
