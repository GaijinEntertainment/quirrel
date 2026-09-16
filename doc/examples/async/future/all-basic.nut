from "async" import Future

async function main() {
  let scout = Future()
  let medic = Future()
  scout.resolve("outpost found")
  medic.resolve("supplies ready")
  let squad = await Future.all([scout, medic])
  println("squad[0] =", squad[0])
  println("squad[1] =", squad[1])
}
main()
