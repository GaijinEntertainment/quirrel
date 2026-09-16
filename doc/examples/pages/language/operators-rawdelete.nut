let activeMissions = { capture_flag = true, defend_base = true }

// the 'delete' operator is forbidden by default (see below); use rawdelete
println("activeMissions.rawdelete(\"capture_flag\") =", activeMissions.rawdelete("capture_flag"))
println("\"capture_flag\" in activeMissions =", "capture_flag" in activeMissions)
println("\"defend_base\" in activeMissions =", "defend_base" in activeMissions)
