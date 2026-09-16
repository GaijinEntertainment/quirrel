const fs = require("fs");
const path = require("path");

const HERE = __dirname;
const EXAMPLES = path.join(HERE, "..", "examples");

const EXPECTED_TO_DIFFER = {
  "globals/__argv.nut":
    "a browser has no command line, so __argv holds the script name alone",
  "debug/format_call_stack_string.nut":
    "the call stack names the file, and in the browser the sample is not one",
  "debug/set_script_watchdog_timeout_msec.nut":
    "the sandbox arms the watchdog before the sample runs, so the previous timeout is not 0",
};

function findExamples(dir, out) {
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const full = path.join(dir, entry.name);
    if (entry.isDirectory()) findExamples(full, out);
    else if (entry.name.endsWith(".nut") && fs.existsSync(full.replace(/\.nut$/, ".out"))) out.push(full);
  }
  return out;
}

function normalize(text) {
  return text.replace(/\r\n/g, "\n").replace(/[ \t]+$/gm, "").replace(/\s+$/, "");
}

async function main() {
  const verbose = process.argv.includes("--verbose");
  const QuirrelVM = require(path.join(HERE, "quirrel.js"));

  let bytes = [];
  const sink = (code) => { if (code !== null) bytes.push(code); };
  const vm = await QuirrelVM({ preRun: [(module) => module.FS.init(null, sink, sink)] });
  console.log("Quirrel " + vm.ccall("quirrel_version", "string", [], []));

  const files = findExamples(EXAMPLES, []).sort();
  const failures = [];
  const stale = [];
  let known = 0;

  for (const file of files) {
    const rel = path.relative(EXAMPLES, file).replace(/\\/g, "/");
    const source = fs.readFileSync(file, "utf8");
    const expected = fs.readFileSync(file.replace(/\.nut$/, ".out"), "utf8");

    bytes = [];
    vm.ccall("quirrel_eval", "number", ["string", "number"], [source, 5000]);
    const actual = new TextDecoder().decode(new Uint8Array(bytes));

    const same = normalize(actual) === normalize(expected);
    if (EXPECTED_TO_DIFFER[rel]) {

      if (same) stale.push(rel);
      else if (verbose) console.log(`known: ${rel} (${EXPECTED_TO_DIFFER[rel]})`);
      known++;
      continue;
    }
    if (!same) failures.push({ rel, expected, actual });
  }

  for (const f of failures) {
    console.log(`\n=== ${f.rel}`);
    console.log("--- expected\n" + f.expected.trimEnd());
    console.log("--- in the browser VM\n" + f.actual.trimEnd());
  }
  for (const rel of stale)
    console.log(`\n${rel} now matches: drop it from EXPECTED_TO_DIFFER`);

  console.log(`\n${files.length} examples, ${failures.length} differ` +
              (known ? `, ${known} known to differ` : ""));
  process.exit(failures.length || stale.length ? 1 : 0);
}

main();
