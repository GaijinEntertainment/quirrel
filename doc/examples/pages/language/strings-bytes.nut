let callsign = "OK"
println("callsign.len() =", callsign.len())

// \x writes raw bytes: a 2-byte UTF-8 character counts as 2, not 1
let playerNick = "\xD0\x9F"
println("playerNick.len() =", playerNick.len())
