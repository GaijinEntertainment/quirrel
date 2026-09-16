from "debug" import type_mask_to_string, get_function_info_table

function add(a: int, b: float) { return a + b }
let info = get_function_info_table(add)

println("type_mask_to_string(argTypeMask[0]) =", type_mask_to_string(info.argTypeMask[0]))
println("type_mask_to_string(argTypeMask[1]) =", type_mask_to_string(info.argTypeMask[1]))

// combining int and float collapses to the "number" alias
println("type_mask_to_string(argTypeMask[0] | argTypeMask[1]) =", type_mask_to_string(info.argTypeMask[0] | info.argTypeMask[1]))

println("type_mask_to_string(-1) =", type_mask_to_string(-1))         // every bit set
println("type_mask_to_string(0) =", $"[{type_mask_to_string(0)}]") // no bit set
