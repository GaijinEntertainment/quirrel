from "async" import Future

// getValue returns the settled value (fulfilled value or fault value), throws
// while pending so null stays a valid settled value (disambiguate via getState).

async function consume(fut) { try { let _ = await fut } catch (_) {} }

let p = Future()
try { let _ = p.getValue(); print("BUG: pending no throw\n") }
catch (e) { println($"pending throws: {e}") }

let f = Future(); f.resolve(42)
println($"fulfilled getValue: {f.getValue()}")        // 42

let n = Future(); n.resolve(null)
println($"null state: {n.getState()} getValue is null: {n.getValue() == null}")               // fulfilled true

let r = Future(); r.reject("boom")
println($"faulted getValue: {r.getValue()}")          // boom
consume(r)   // mark the fault handled so it is not reported as unhandled

print("script done\n")
