from "string" import escape

// backslash and the two quote characters keep a one-letter escape
println("escape(quotes and backslash) =", escape("say \"hi\"\\bye"))

// every other non-printable byte, including a plain tab or newline,
// becomes \xNN, not the classic C \t \n letter form
println("escape(tab and newline) =", escape("a\tb\nc"))

// nothing to escape: the same string comes back
println("escape(\"plain\") == \"plain\" =", escape("plain") == "plain")
println("escape(\"\") == \"\" =", escape("") == "")
