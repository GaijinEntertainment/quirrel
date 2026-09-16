from "iostream" import blob

let b = blob(2)
println("b.eos() at start =", b.eos())        // cursor 0, len 2: not at the end
b.readn('b')
println("b.eos() after 1 readn('b') =", b.eos())        // cursor 1, len 2: still not there
b.readn('b')
println("b.eos() after 2 readn('b') =", b.eos())        // cursor 2, len 2: now at the end

b.writen(9, 'b')         // writing past the end grows len, so eos flips back
println("b.eos() after writen past end =", b.eos())
