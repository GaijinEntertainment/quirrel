error("squad wiped:", "no ammo left")   // goes to stderr, not stdout
println("\nstdout: still running")       // error() does not stop the script; adds no newline of its own
