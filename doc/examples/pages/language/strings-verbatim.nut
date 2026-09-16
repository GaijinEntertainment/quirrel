// @"..." takes every character literally; double a quote to put one in
let modelPath = @"content\tanks\""t34""\turret.dag"
println(modelPath)

// and, unlike an ordinary string, it may span real newlines. The break is the
// one in the file, so a CRLF file puts a \r in the string: strip each line
let briefing = @"first wave: north ridge
second wave: south gate"
foreach (line in briefing.split("\n"))
  println(line.strip())
