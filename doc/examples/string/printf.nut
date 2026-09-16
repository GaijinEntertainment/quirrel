from "string" import printf

// same formatting rules as format(), but writes the result instead of
// returning it, without building a throwaway string first
printf("%s = %d\n", "answer", 42)

// the call itself evaluates to null
println(printf("%d\n", 1) == null)
