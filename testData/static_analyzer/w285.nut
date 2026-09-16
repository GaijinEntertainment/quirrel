if (__name__ == "__analysis__")
  return

//-file:undefined-global
//-file:declared-never-used


local uu = ::sys.gh("fff") ?? ""
if ((uu ?? "") != "")
  print($"x: {uu}")


local regions = ::unlock?.meta.regions ?? [::unlock?.meta.region] ?? []

local regions2 = ::x ? [] : {}
let _g = regions2 ?? 123


local regions3 = ::x ? 2 : 4
let _h = regions3 != null

// This caused a false positive, but the bug was fixed
function _foo(a=null){
  let b = a
  a = a ?? 3
  return [b ?? 2, a]
}

function _countSubStr(row, subStr) {
  local res = 0
  local i = -subStr.len()
  while (i != null) {
    i = row.indexof(subStr, i + subStr.len())
    if (i != null)
      res++
  }
  return res
}

function _countSubStrWithFor(row, subStr) {
  local res = 0
  for (local i = -subStr.len(); i != null; i = row.indexof(subStr, i + subStr.len()))
    res++
  return res
}

function _checkNullableLoopCondition(row) {
  local pos = row.indexof("x")
  while (pos != null && pos < 10)
    pos = pos + 1
}

function _checkLoopCondition() {
  local x = 5
  while (x != null)
    x++
}
