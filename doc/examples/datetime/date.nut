from "datetime" import date

// a fixed time forced to UTC gives the same table on every machine
let d = date(0, 'u')
println("d.year =", d.year)
println("d.month =", d.month)
println("d.day =", d.day)
println("d.wday =", d.wday)
