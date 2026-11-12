//-file:declared-never-used

let t = {a = 1, b = 2}
let cfg = { inner = { c = 3 } }
class K { static s = 1 }

let a = t.__merge({}) //warning:merge-empty-table

let dotted = cfg.inner.__merge({}) //warning:merge-empty-table

let b = t.__merge({x = 1}) // ok, non-empty table

let c = clone t // ok, already using clone

let k = K.__merge({}) // ok, a class merge answers a class, a spread would answer a table

let n1 = t?.__merge({}) // ok, the call answers null for a null receiver

let n2 = cfg?.inner.__merge({}) // ok, the chain reads through a nullable access
