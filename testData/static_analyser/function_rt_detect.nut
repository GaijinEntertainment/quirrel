
// -file:plus-string

function trim(s) { return s }

function _foo(_p) { // OK
    if (_p)
        return 1
    else
        return 1 + trim("A") + trim("B") + trim("C").join(true) + 9
}

function _bar(_p) { // WRONG
    if (_p)
        return "1"
    else
        return 2 + trim("A") + trim("B") + trim("C").join(true) + 8
}