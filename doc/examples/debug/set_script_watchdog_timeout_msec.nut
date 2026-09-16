from "debug" import set_script_watchdog_timeout_msec

println("set_script_watchdog_timeout_msec(5000) =", set_script_watchdog_timeout_msec(5000)) // 0: no timeout was set yet
println("set_script_watchdog_timeout_msec(0) =", set_script_watchdog_timeout_msec(0))    // 5000: the value just set
