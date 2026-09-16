const fs = require("fs");
const path = require("path");

const HERE = __dirname;
const ROOT = path.join(HERE, "..");
const SITE = path.join(ROOT, "_site");
const EXAMPLES = path.join(ROOT, "examples");

function findExamples(dir, out) {
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const full = path.join(dir, entry.name);
    if (entry.isDirectory()) findExamples(full, out);
    else if (entry.name.endsWith(".nut")) out.push(full);
  }
  return out;
}

function loadBrowserHighlighter() {

  const window = {};
  const sandbox = { window, document: null, NodeFilter: {} };
  for (const name of ["lexer.js", "highlight.js"]) {
    const src = fs.readFileSync(path.join(SITE, name), "utf8");
    new Function("window", "document", "NodeFilter", src)(
      sandbox.window, sandbox.document, sandbox.NodeFilter);
  }
  return window.quirrelEditor.highlight;
}

function main() {
  if (!fs.existsSync(path.join(SITE, "lexer.js"))) {
    console.error("run python build.py first: _site/lexer.js is not there");
    process.exit(2);
  }
  const highlight = loadBrowserHighlighter();

  const expected = new Map();
  const walk = (dir) => {
    for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
      const full = path.join(dir, entry.name);
      if (entry.isDirectory()) walk(full);
      else if (entry.name.endsWith(".html")) {
        const html = fs.readFileSync(full, "utf8");
        for (const m of html.matchAll(/<pre class="code"><code[^>]*>([\s\S]*?)<\/code><\/pre>/g))
          expected.set(m[1], full);
      }
    }
  };
  walk(SITE);

  const files = findExamples(EXAMPLES, []).sort();
  const failures = [];
  let checked = 0;

  for (const file of files) {

    const source = fs.readFileSync(file, "utf8").replace(/\r\n/g, "\n").replace(/\n+$/, "");
    const mine = highlight(source);
    if (!expected.has(mine)) {

      const rel = path.relative(ROOT, file).replace(/\\/g, "/");
      failures.push(rel);
    } else {
      checked++;
    }
  }

  for (const rel of failures) console.log("differs from the built page: " + rel);
  console.log(`${checked} of ${files.length} examples highlight identically`);
  process.exit(failures.length ? 1 : 0);
}

main();
