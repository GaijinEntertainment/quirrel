from "async" import Future

async function main() {
  let a = Future(); let b = Future()
  b.resolve("B"); a.resolve("A")     // settle out of order
  let r = await Future.all([a, b])
  print($"order: {r[0]}{r[1]}\n")  // still input-ordered

  let ok = Future(); ok.resolve("x")
  let bad = Future(); bad.reject("boom")
  try { await Future.all([ok, bad]) }
  catch (e) { print($"fail-fast: {e}\n") }
}
main()
