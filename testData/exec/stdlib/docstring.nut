from "test.native" import raw_cmp, set_object_docstring
let debug = require("debug")

function noDocFunction() {}
class NoDocClass {}

class DocumentedClass {
  @@"Class docstring"
}

class DerivedWithoutDoc(DocumentedClass) {}

class DerivedWithDoc(DocumentedClass) {
  @@"Derived docstring"
}

function documentedFunction() {
  @@"Function docstring"
  return 1234
}

function emptyDocFunction() {
  @@""
}

function makeDocumentedFunction() {
  return function() {
    @@"Shared function docstring"
    return 1
  }
}

function makeDocumentedClass() {
  return class {
    @@"Repeated class docstring"
  }
}

let firstFunction = makeDocumentedFunction()
let secondFunction = makeDocumentedFunction()
let firstClass = makeDocumentedClass()
let secondClass = makeDocumentedClass()
let noDocInstanceBeforeSetter = NoDocClass()

println(debug.doc(noDocFunction))
println(debug.doc(NoDocClass))
println(debug.doc(noDocInstanceBeforeSetter))
println(debug.doc(DocumentedClass))
println(debug.doc(DocumentedClass()))
println(debug.doc(DerivedWithoutDoc))
println(debug.doc(DerivedWithoutDoc()))
println(debug.doc(DerivedWithDoc))
println(debug.doc(DerivedWithDoc()))
println(debug.doc(documentedFunction))
println(debug.doc(firstFunction))
println(debug.doc(secondFunction))
println(debug.doc(firstClass))
println(debug.doc(secondClass()))
println(debug.doc(emptyDocFunction) == "")
println(documentedFunction.getfuncinfos().doc == debug.doc(documentedFunction))
println(debug.get_function_info_table(documentedFunction).doc == debug.doc(documentedFunction))
println(debug.doc(debug.doc))
println(debug.doc(raw_cmp))
println(set_object_docstring(raw_cmp, "Native setter docstring"))
println(debug.doc(raw_cmp))
println(set_object_docstring(noDocFunction, "Script setter docstring"))
println(debug.doc(noDocFunction))
println(set_object_docstring(NoDocClass, "Class setter docstring"))
println(debug.doc(noDocInstanceBeforeSetter))

try {
  debug.doc({})
}
catch (_error) {
  println("table rejected")
}

try {
  set_object_docstring({}, "bad")
}
catch (_error) {
  println("table setter rejected")
}

try {
  set_object_docstring(NoDocClass(), "bad")
}
catch (_error) {
  println("instance setter rejected")
}
