function announceArrival(courierName) { return $"{courierName} has arrived" }
let t = newthread(announceArrival)
println("t.call(\"scout\") =", t.call("scout"))
