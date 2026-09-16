function sentryDuty() { suspend() }
let watchpost = newthread(sentryDuty)
watchpost.call()
println("watchpost.getstackinfos(0).func =", watchpost.getstackinfos(0).func)
