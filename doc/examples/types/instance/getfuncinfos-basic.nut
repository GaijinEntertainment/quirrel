class Blaster {
  function _call(target) { return target != null }
}
let info = Blaster().getfuncinfos()
println("info.name =", info.name)
