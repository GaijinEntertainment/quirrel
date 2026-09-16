let hitPoints = 40

// if on its own
if (hitPoints < 100)
  println("damaged")

// if with an else
if (hitPoints > 0)
  println("alive")
else
  println("destroyed")

// a chain: the first matching branch wins, the rest are skipped
if (hitPoints > 75)
  println("condition: green")
else if (hitPoints > 25)
  println("condition: yellow")
else
  println("condition: red")
