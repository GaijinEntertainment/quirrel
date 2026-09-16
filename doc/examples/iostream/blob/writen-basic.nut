from "iostream" import blob

let ammo = blob(0)
ammo.writen(200, 'i')

ammo.seek(0)
println("ammo.readn('i') =", ammo.readn('i'))
