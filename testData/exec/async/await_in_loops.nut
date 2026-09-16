from "async" import Future

// `await` inside for / foreach. The codegen RM_PLAIN path yields loop
// counters/iterators through their own stack slot; this test exercises both
// loop forms with an OP_YIELD inside the body so a regression in counter
// preservation across the suspension would surface here.

let p = Future()
p.resolve("ok")

async function main() {
  for (local i = 0; i < 3; i++) {
    let v = await p
    println($"for i={i} v={v}")
  }

  let arr = ["a", "b", "c"]
  foreach (idx, item in arr) {
    let v = await p
    println($"foreach idx={idx} item={item} v={v}")
  }
}

main()
print("script done\n")
