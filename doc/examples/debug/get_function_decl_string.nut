from "debug" import get_function_decl_string

function add(a, b: int) { return a + b }
println("get_function_decl_string(add) =", get_function_decl_string(add))

// array.insert has no decl string on record, so one is rebuilt from its type
// mask, using arg1, arg2 in place of the real parameter names
println("get_function_decl_string([].insert) =", get_function_decl_string([].insert))

try {
  get_function_decl_string("not a function")
} catch (e) {
  println("get_function_decl_string(\"not a function\") throws:", e)
}
