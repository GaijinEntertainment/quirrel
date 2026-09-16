from "async" import Future

// await unwraps exactly ONE level (no adoption). A Future resolved with another
// Future yields that inner Future; a second await reaches the value. An async
// function that returns a Future fulfils with it verbatim; `return await`
// forwards the value instead.

async function returnsFuture() { let f = Future(); f.resolve(9); return f }
async function returnsValue()  { let f = Future(); f.resolve(9); return await f }

async function main() {
    let inner = Future(); inner.resolve(7)
    let outer = Future(); outer.resolve(inner)   // stored verbatim
    let got = await outer
    println($"got is inner: {got == inner}")          // true
    println($"got.getValue: {got.getValue()}")        // 7
    println($"deep: {await got}")                     // 7

    let a = await returnsFuture()                     // a is the inner Future
    println($"a.getState: {a.getState()}")            // fulfilled
    println($"a deep: {await a}")                     // 9
    println($"return await: {await returnsValue()}")         // 9
}
main()
print("script done\n")
