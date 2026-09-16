from "iostream" import blob

let b = blob(0)
println("b.eos() at start =", b.eos())         // len 0, cursor 0: already at the end
b.writen(0x11223344, 'i')
println("b.eos() after writen =", b.eos())         // the write advanced the cursor exactly to the new len
b.seek(0)
println("b.eos() after seek(0) =", b.eos())         // cursor moved back, len did not
b.seek(0, 'e')
println("b.eos() after seek(0, 'e') =", b.eos())
