from "debug" import getstackinfos

function inner() {
  let info = getstackinfos(1)   // 1 = the caller of getstackinfos itself
  println("info.func =", info.func)
}
inner()
