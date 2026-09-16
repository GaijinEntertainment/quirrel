//-file:declared-never-used

local function id(a) { return a }

local function multiStatementLine() {
  local a = 1; a = 2
  print(a)
}

local function statementAfterMultiStatementLine() {
  local a = 1
  a = 2; print(a)
  print(a)
}

local function multiLineOpener() {
  local a = id(
    1
  ); local b = 2
  print(a)
  print(b)
}

local function misindentAfterMultiStatementLine() {
  local a = 1; a = 2
    print(a)
}
