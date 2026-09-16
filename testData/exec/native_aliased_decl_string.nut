// One C function bound to two slots must keep a decl string and a docstring per
// slot, not per function pointer. A bound clone shares its source registry pair.

from "test.native" import aliased_first, aliased_second, doc_registry_slot_count, raw_cmp, set_object_docstring
let { doc, get_function_decl_string, get_function_info_table, collectgarbage } = require("debug")

println(get_function_decl_string(aliased_first))
println(get_function_decl_string(aliased_second))

println(doc(aliased_first))
println(doc(aliased_second))
println(set_object_docstring(aliased_second, "updated second doc"))
println(get_function_decl_string(aliased_second))
println(doc(aliased_second))

println(get_function_info_table(aliased_first).functionName)
println(get_function_info_table(aliased_second).functionName)
println(get_function_info_table(aliased_first).requiredArgs)
println(get_function_info_table(aliased_second).requiredArgs)
println(get_function_info_table(aliased_first).doc)
println(require("debug").getbuildinfo().docstring_registry_slots == doc_registry_slot_count())

let params1 = aliased_first.getfuncinfos().parameters
let params2 = aliased_second.getfuncinfos().parameters
println($"{params1.len()} {params2.len()} {params2[1]}")
println(aliased_first.getfuncinfos().doc)

// A docstring set without a decl string is keyed the same way
println(doc(doc_registry_slot_count))

// A bound copy keeps both entries of the closure it was made from. Without them
// the decl string falls back to what the closure fields can rebuild (no return
// type) and the docstring is lost entirely
let bound = aliased_first.bindenv({})
println(get_function_decl_string(bound))
println(doc(bound))

// A native with no entries at all stays that way through bindenv
println(doc(raw_cmp.bindenv({})))

// Clones share the append-only pair and do not add registry slots.
function makeAndDropCopies() {
  let copies = []
  for (local i = 0; i < 100; i++)
    copies.append(aliased_first.bindenv({}))
  let live = doc_registry_slot_count()
  copies.clear()
  collectgarbage()
  return [live, doc_registry_slot_count()]
}

let before = doc_registry_slot_count()
let round1 = makeAndDropCopies()
let round2 = makeAndDropCopies()
println(round1[0] - before)
println(round2[1] - round1[1])
// This value pins one slot per script docstring and two per native pair.
println(doc_registry_slot_count() > 1)

println("PASSED")
