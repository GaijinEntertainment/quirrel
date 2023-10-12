//-file:declared-never-used


let c = {}


foreach (a in c) {
    delete c.x
    c.rawdelete("y")
}