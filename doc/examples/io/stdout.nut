from "io" import stdout

println("via println")
stdout.writestring("via stdout.writestring\n")   // same underlying stream as println

stdout.close()                                    // shared, not owned: this is a no-op
stdout.writestring("still open after close\n")
