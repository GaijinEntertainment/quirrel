from "debug" import type_mask_to_string, get_function_info_table

function heal(hitPoints: int) { return hitPoints }
let mask = get_function_info_table(heal).argTypeMask[0]

println("type_mask_to_string(mask) =", type_mask_to_string(mask))
