from "async" import Future

async function main() {
  // `loser` is array-first but never settles, so the winner is decided by
  // settle order, not array order.
  let winner = Future(); let loser = Future()
  winner.resolve("first")
  print($"fulfil: {await Future.race([loser, winner])}\n")

  let bad = Future(); let pending = Future()
  bad.reject("boom")
  try { await Future.race([pending, bad]) }
  catch (e) { print($"fault: {e}\n") }
}
main()
