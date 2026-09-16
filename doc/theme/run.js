(function () {
  const root = "../".repeat(Number(document.documentElement.dataset.depth || 0));
  const examples = Array.from(document.querySelectorAll(".example"))
    .filter((ex) => ex.querySelector(".run"));
  if (!examples.length) return;

  const buttons = examples.map((ex) => ex.querySelector(".run"));

  const TIMEOUT_MSEC = 2000;

  const RUN_KEYS = (ev) => (ev.key === "Enter" && (ev.ctrlKey || ev.metaKey)) || ev.key === "F9";

  const KILL_MSEC = TIMEOUT_MSEC + 3000;

  let worker = null;
  let ready = false;
  let pending = null;

  const RUN_TITLE = "run it (Ctrl+Enter)";
  const WAIT_TITLE = "another sample is running";

  function limitOf(example) {
    return example.querySelector(".runlimit");
  }

  function note(example, text) {
    const span = limitOf(example);
    if (span) span.textContent = text;
  }

  function busy(button, text) {
    buttons.forEach((b) => {
      b.disabled = true;
      b.title = b === button ? RUN_TITLE : WAIT_TITLE;
      if (b === button) b.textContent = text;
      else note(b.closest(".example"), WAIT_TITLE);
    });
  }

  function idle() {
    buttons.forEach((b) => {
      b.textContent = "Run this code";
      b.disabled = false;
      b.title = RUN_TITLE;
      note(b.closest(".example"), "");
    });
  }

  function outputBlock(example) {
    return example.querySelector("pre.out code");
  }

  function expectedOf(example) {
    const out = outputBlock(example);
    if (out && out.dataset.expected === undefined) out.dataset.expected = out.textContent;
    return out ? out.dataset.expected : "";
  }

  function show(example, text, note, bad) {
    const out = outputBlock(example);
    if (!out) return;
    out.textContent = text;
    const label = example.querySelector(".outlabel");
    if (label) {
      label.textContent = "Output: ";
      const tag = document.createElement("span");
      tag.className = bad ? "runtag bad" : "runtag";
      tag.textContent = note;
      label.appendChild(tag);
    }
  }

  function finish(example, output) {

    const same = output.replace(/\s+$/, "") === expectedOf(example).replace(/\s+$/, "");
    show(example, output,
         same ? "ran in your browser" : "ran in your browser, differs from the checked output",
         !same);
  }

  function sourceOf(example) {
    const code = example.querySelector("pre.code code");
    return code ? code.textContent : "";
  }

  function newWorker() {
    ready = false;
    worker = new Worker(root + "run_worker.js");
    worker.onerror = unavailable;
    worker.onmessage = (ev) => {
      const msg = ev.data;
      if (msg.type === "unavailable") return unavailable();
      if (msg.type === "ready") {
        ready = true;
        const queued = pending;
        pending = null;
        idle();
        if (queued) start(queued.example, queued.button);
        return;
      }
      if (!pending) return;
      clearTimeout(pending.timer);
      const example = pending.example;
      pending = null;
      idle();
      if (msg.type === "crashed") {

        worker.terminate();
        newWorker();
        show(example, msg.output, "the VM stopped", true);
      } else {
        finish(example, msg.output);
      }
    };
    worker.postMessage({ cmd: "init" });
  }

  function unavailable() {
    if (worker) worker.terminate();
    worker = null;
    pending = null;
    buttons.forEach((b) => {
      b.textContent = "in-browser VM not available";
      b.disabled = true;
      b.title = "this browser did not start the WebAssembly VM";
      note(b.closest(".example"), "run the sample with the sq interpreter instead");
    });
  }

  function start(example, button) {
    pending = {
      example: example,
      button: button,
      timer: setTimeout(() => {

        const stuck = pending.example;
        pending = null;
        worker.terminate();
        newWorker();
        idle();
        show(stuck, "stopped: the sample did not finish", "stopped", true);
      }, KILL_MSEC),
    };
    busy(button, "running...");
    worker.postMessage({ cmd: "run", source: sourceOf(example), timeout: TIMEOUT_MSEC });
  }

  function request(example, button) {
    expectedOf(example);
    if (!worker || pending) return;
    if (!ready) {

      pending = { example: example, button: button, timer: 0 };
      busy(button, "loading the VM...");
      return;
    }
    start(example, button);
  }

  function addReset(example, code, original) {
    const bar = example.querySelector(".exbar");
    if (!bar) return;
    const link = document.createElement("button");
    link.className = "reset";
    link.textContent = "reset";
    link.title = "put the documented sample back";
    link.addEventListener("click", () => {
      code.innerHTML = original;
      link.remove();
      const out = example.querySelector("pre.out code");
      const label = example.querySelector(".outlabel");
      if (out && out.dataset.expected !== undefined) out.textContent = out.dataset.expected;
      if (label) label.textContent = "Output:";
    });
    bar.insertBefore(link, example.querySelector(".run"));
  }

  examples.forEach((example) => {
    const button = example.querySelector(".run");
    button.title = RUN_TITLE;

    const out = example.querySelector("pre.out");
    if (out) {
      out.setAttribute("aria-live", "polite");
      out.setAttribute("aria-atomic", "true");
    }
    const limit = document.createElement("span");
    limit.className = "runlimit";
    button.parentNode.insertBefore(limit, button);

    const code = example.querySelector("pre.code code");
    if (code && window.quirrelEditor) {
      window.quirrelEditor.makeEditable(code);
      code.title = "editable - change it and run it with Ctrl+Enter";

      const original = code.innerHTML;
      let offered = false;
      code.addEventListener("input", () => {
        if (offered) return;
        offered = true;
        addReset(example, code, original);
      });

      code.addEventListener("keydown", (ev) => {
        if (!RUN_KEYS(ev)) return;
        ev.preventDefault();
        request(example, button);
      });
    }

    button.addEventListener("click", () => request(example, button));
  });

  newWorker();
})();
