from "io" import file

let path = "definitely/does/not/exist.bin"

try { file(path, "zz"); }        // mode is checked before the path is ever touched
catch (e) { println("file(path, \"zz\") throws:", e); }

try { file(path, "r"); }         // mode is fine now, but the path still is not there
catch (e) { println("file(path, \"r\") throws:", e); }
