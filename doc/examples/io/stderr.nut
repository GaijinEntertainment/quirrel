from "io" import stdout, stderr

stdout.writestring("stdout line\n")
stderr.writestring("stderr line\n")   // a separate stream, merged into the same log by the shell

println(typeof stderr)                // "file": stderr is an io.file instance, not a special type
