from "iostream" import blob

let hp = blob(0)
hp.writen(100, 'i')

hp.seek(0)
println("hp.readn('i') =", hp.readn('i'))
