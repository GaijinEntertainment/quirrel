from "iostream" import blob

let b = blob(4)
b.writen(0x01020304, 'i')
b.seek(3)                 // only 1 byte remains before len

let got = b.readblob(4) // asking for more than remains is not an error
println("got.len() =", got.len())        // fewer bytes than asked for came back

try {
  b.readblob(1)            // now nothing at all remains
} catch (e) {
  println("b.readblob(1) throws:", e)
}
