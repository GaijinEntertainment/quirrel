from "async" import Future

async function main() {
  let scout = Future()
  let medic = Future()
  scout.resolve("scout")   // already settled; medic never does
  println("await Future.race([scout, medic]) =", await Future.race([scout, medic]))
}
main()
