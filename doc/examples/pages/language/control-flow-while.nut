// while checks before each pass: zero rounds skips the loop entirely
local rounds = 0
while (rounds > 0) {
  println("firing")
  rounds -= 1
}
println($"rounds left: {rounds}")

// do/while checks after: the body always runs at least once
local reloadAttempt = 0
do {
  reloadAttempt += 1
  println($"reload attempt {reloadAttempt}")
} while (reloadAttempt < 3)
