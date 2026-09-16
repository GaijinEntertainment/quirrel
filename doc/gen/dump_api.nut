// Dumps the native API of the running host, for doc/build.py.
//
//   csq-dev.exe doc/gen/dump_api.nut > doc/gen/api_dump.jsonl
//
// The host binary decides what is in the dump: csq exposes the Dagor module set,
// sq exposes core Quirrel only.
//
// Output is JSON Lines, one object per line, because the host print function
// breaks a line longer than 64K. Table iteration order is randomized, so every
// member list is sorted; the output must be byte-stable across runs.

let dbg = require("debug")
let modulesLib = require_optional("modules")

let { get_function_decl_string, get_function_info_table, doc, getbuildinfo } = dbg

// csq registers `modules`, sq.exe does not. Without it we can still walk the root
// table, so a missing module list degrades instead of failing.
let moduleNames = modulesLib != null ? modulesLib.get_native_module_names() : []

function esc(s) {
  local r = s.replace("\\", "\\\\")
  r = r.replace("\"", "\\\"")
  r = r.replace("\n", "\\n")
  r = r.replace("\r", "\\r")
  r = r.replace("\t", "\\t")
  return r
}

function jstr(s) {
  return s == null ? "null" : "".concat("\"", esc(s), "\"")
}

function jbool(b) {
  return b ? "true" : "false"
}

function jarr(a, quoted) {
  let parts = []
  foreach (v in a)
    parts.append(quoted ? jstr(v.tostring()) : v.tostring())
  return "".concat("[", ",".join(parts), "]")
}

function obj(fields) {
  return "".concat("{", ",".join(fields), "}")
}

function sortedKeys(container) {
  let keys = []
  foreach (k, _ in container)
    keys.append(k.tostring())
  keys.sort()
  return keys
}

// A member is emitted with whatever the VM knows about it. Functions carry the
// full introspection table; constants carry their value so the docs can show it.
function emitMember(containerId, name, value) {
  let kind = type(value)
  let fields = [
    "\"row\":\"member\"",
    "".concat("\"container\":", jstr(containerId)),
    "".concat("\"name\":", jstr(name)),
    "".concat("\"kind\":", jstr(kind))
  ]

  if (kind == "function") {
    let info = get_function_info_table(value)
    fields.append("".concat("\"decl\":", jstr(get_function_decl_string(value))))
    if (info != null) {
      fields.append("".concat("\"doc\":", jstr(info.doc)))
      fields.append("".concat("\"native\":", jbool(info.native)))
      fields.append("".concat("\"pure\":", jbool(info.pure)))
      fields.append("".concat("\"fastcall\":", jbool(info.fastcall)))
      fields.append("".concat("\"nodiscard\":", jbool(info.nodiscard)))
      fields.append("".concat("\"requiredArgs\":", info.requiredArgs.tostring()))
      fields.append("".concat("\"returnTypeMask\":", info.returnTypeMask.tostring()))
      fields.append("".concat("\"objectTypeMask\":", info.objectTypeMask.tostring()))
      fields.append("".concat("\"ellipsisArgTypeMask\":", info.ellipsisArgTypeMask.tostring()))
      fields.append("".concat("\"argNames\":", jarr(info.argNames, true)))
      fields.append("".concat("\"argTypeMask\":", jarr(info.argTypeMask, false)))
    }
  }
  else if (kind == "class") {
    fields.append("".concat("\"doc\":", jstr(doc(value))))
  }
  else if (kind == "instance") {
    fields.append("".concat("\"doc\":", jstr(doc(value))))
  }
  else if (kind == "integer" || kind == "float" || kind == "string" || kind == "bool") {
    fields.append("".concat("\"value\":", jstr(value.tostring())))
  }

  println(obj(fields))
}

// Emits one container (root table, module or class) and returns the classes found
// inside it, so the caller can emit them as containers of their own.
function emitContainer(id, kind, container, docString) {
  println(obj([
    "\"row\":\"container\"",
    "".concat("\"id\":", jstr(id)),
    "".concat("\"kind\":", jstr(kind)),
    "".concat("\"doc\":", jstr(docString))
  ]))

  let nested = []
  foreach (name in sortedKeys(container)) {
    let value = container[name]
    emitMember(id, name, value)
    if (type(value) == "class")
      nested.append({ id = "".concat(id, ".", name), value = value })
  }
  return nested
}

let build = getbuildinfo()

println(obj([
  "\"row\":\"header\"",
  "".concat("\"quirrelVersion\":", jstr(build?.version ?? "unknown"))
]))

emitContainer("", "root", getroottable(), null)

foreach (moduleName in moduleNames.sort()) {
  let mod = require_optional(moduleName)
  let modType = type(mod)

  // A module usually exports a table, but some export a single class (DataBlock).
  if (modType == "class") {
    emitContainer(moduleName, "class", mod, doc(mod))
    continue
  }
  if (modType != "table")
    continue

  foreach (cls in emitContainer(moduleName, "module", mod, null))
    emitContainer(cls.id, "class", cls.value, doc(cls.value))
}
