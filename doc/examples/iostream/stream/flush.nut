from "iostream" import blob

let b = blob(0)
b.writen(1, 'i')
println(b.flush()) // a blob has nothing buffered to flush; this always succeeds
