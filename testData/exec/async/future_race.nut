from "async" import Future

// Future.race: the first input to settle wins, for both fulfilment and fault.

async function main() {
  // Genuine stagger: `loser` (array-first) never settles, `winner` fulfils, so
  // race resolves with the winner -- proving it is settle-time, not array order.
  {
    let winner = Future()
    let loser = Future()  // never settles
    async function go() { winner.resolve("winner") }
    go()
    let r = await Future.race([loser, winner])
    println($"race fulfil: {r}")
  }

  // First to fault wins: `boom` faults, the other input never settles, so the
  // child catches it and does done.reject(e), and the await on race throws.
  {
    async function boom() { throw "race-boom" }
    let pending = Future()  // never settles
    try {
      let r = await Future.race([pending, boom()])
      println($"UNEXPECTED fulfil: {r}")
    } catch (e) {
      println($"race fault: {e}")
    }
  }

  // Tie-break: both inputs already settled before any child runs. Children resume
  // in array order, so the array-first input wins (matches JS Promise.race).
  {
    let r1 = Future()
    let r2 = Future()
    r1.resolve("A")
    r2.resolve("B")
    let r = await Future.race([r1, r2])
    println($"race tiebreak: {r}")
  }

  // Non-future element resumes its child immediately (await passthrough), so it
  // wins the race over a still-pending future input.
  {
    let pending = Future()  // never settles
    let r = await Future.race([pending, 42])
    println($"race passthrough: {r}")
  }

  print("script done\n")
}
main()
