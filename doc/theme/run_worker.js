const LINE_LIMIT = 60;
const OUTPUT_LIMIT = 256 * 1024;

let vm = null;
let bytes = [];
let lines = 0;
let dropped = false;

function sink(code) {
  if (code === null) return;
  if (lines >= LINE_LIMIT || bytes.length >= OUTPUT_LIMIT) {
    dropped = true;
    return;
  }
  bytes.push(code);
  if (code === 10) lines++;
}

function taken() {
  const text = new TextDecoder().decode(new Uint8Array(bytes));
  if (!dropped) return text;
  const why = lines >= LINE_LIMIT
    ? "... output truncated at " + LINE_LIMIT + " lines"
    : "... output truncated";
  return text.endsWith("\n") ? text + why : text + "\n" + why;
}

self.onmessage = (ev) => {
  const msg = ev.data;

  if (msg.cmd === "init") {

    try {
      importScripts("quirrel.js");
    } catch (e) {
      self.postMessage({ type: "unavailable" });
      return;
    }
    QuirrelVM({
      preRun: [(module) => module.FS.init(null, sink, sink)],
    }).then(
      (module) => {
        vm = module;
        self.postMessage({ type: "ready", version: vm.ccall("quirrel_version", "string", [], []) });
      },
      () => self.postMessage({ type: "unavailable" })
    );
    return;
  }

  if (msg.cmd === "run") {
    bytes = [];
    lines = 0;
    dropped = false;
    try {
      vm.ccall("quirrel_eval", "number", ["string", "number"], [msg.source, msg.timeout]);
      self.postMessage({ type: "done", output: taken() });
    } catch (e) {

      self.postMessage({ type: "crashed", output: String((e && e.message) || e) });
    }
  }
};
