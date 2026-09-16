(function () {
  const root = "../".repeat(Number(document.documentElement.dataset.depth || 0));
  const box = document.getElementById("q");
  const panel = document.getElementById("results");
  const status = document.getElementById("results-status");
  const list = document.getElementById("results-list");
  const more = document.getElementById("results-more");
  const LIMIT = 40;
  let index = null;
  let loading = null;
  let failed = false;
  let selected = 0;
  let hits = [];
  let capped = false;

  function load() {
    if (index) return Promise.resolve(index);
    if (loading) return loading;
    failed = false;
    loading = fetch(root + "search.json")
      .then((r) => {
        if (!r.ok) throw new Error(r.status + " " + r.statusText);
        return r.json();
      })
      .then((data) => {
        index = data;
        loading = null;
        return data;
      })
      .catch((err) => {
        loading = null;
        failed = true;
        throw err;
      });
    return loading;
  }

  function fileName(entry) {
    if (entry.file === undefined)
      entry.file = entry.url.toLowerCase().split("#")[0].replace(/\.html$/, "");
    return entry.file;
  }

  function score(entry, needle) {
    const name = entry.name.toLowerCase();
    const ref = entry.ref.toLowerCase();
    const section = entry.kind === "section";

    if (!section) {
      if (name === needle || ref === needle) return 0;
      if (name.startsWith(needle)) return 1;
      if (ref.startsWith(needle)) return 2;
      if (name.includes(needle)) return 3;
      if (ref.includes(needle)) return 4;
    } else {
      if (ref === needle) return 1;
      if (ref.startsWith(needle)) return 3;

      const words = needle.split(/\s+/).filter(Boolean);
      if (words.length > 1 && words.every((w) => ref.includes(w))) return 4;
      if (ref.includes(needle)) return 5;
    }
    if ((entry.doc || "").toLowerCase().includes(needle)) return 6;
    if (fileName(entry).includes(needle)) return 7;
    return -1;
  }

  const KIND_BIAS = {
    keyword: -0.3, operator: -0.3, metamethod: -0.25,
    symbol: 0, module: 0.1, class: 0.1, page: 0.1, native: 0.15, section: 0.2,
  };

  function close() {
    panel.hidden = true;
    tail("");
    box.setAttribute("aria-expanded", "false");
    box.removeAttribute("aria-activedescendant");
  }

  function say(text) {
    status.textContent = text;
    status.hidden = !text;
  }

  function tail(text) {
    more.textContent = text;
    more.hidden = !text;
  }

  function show(note) {
    list.innerHTML = "";
    tail("");
    say(note);
    panel.hidden = false;
    box.setAttribute("aria-expanded", "false");
    box.removeAttribute("aria-activedescendant");
  }

  function render() {
    if (!hits.length) {
      show("Nothing matches " + box.value.trim());
      return;
    }
    say("");
    tail(capped ? "Showing the first " + LIMIT + " of many; make the query narrower" : "");
    list.innerHTML = hits
      .map((h, i) => {
        const kind = h.kind === "symbol" ? "" : `<span class="r-kind">${h.kind}</span>`;
        return (
          `<a id="r-${i}" role="option" aria-selected="${i === selected}"` +
          ` class="${i === selected ? "sel" : ""}" href="${root}${h.url}">` +
          `${kind}<span class="r-ref">${h.ref}</span>` +
          `<div class="r-doc">${h.doc || h.sig || ""}</div></a>`
        );
      })
      .join("");
    panel.hidden = false;
    box.setAttribute("aria-expanded", "true");
    box.setAttribute("aria-activedescendant", "r-" + selected);
    const sel = list.querySelector(".sel");
    if (sel) sel.scrollIntoView({ block: "nearest" });
  }

  function match(needle) {
    hits = index
      .map((e) => ({ e, s: score(e, needle) }))
      .filter((x) => x.s >= 0)
      .map((x) => ({ e: x.e, s: x.s + (KIND_BIAS[x.e.kind] || 0) }))
      .sort((a, b) => a.s - b.s || a.e.ref.length - b.e.ref.length);
    capped = hits.length > LIMIT;
    hits = hits.slice(0, LIMIT).map((x) => x.e);
    selected = 0;
    render();
  }

  function search() {
    const needle = box.value.trim().toLowerCase();
    remember(box.value.trim());
    if (!needle) {
      hits = [];
      close();
      return;
    }
    if (index) {
      match(needle);
      return;
    }
    show(failed ? "Loading the index again..." : "Loading the index...");
    load()
      .then(() => {
        if (box.value.trim().toLowerCase() === needle) match(needle);
      })
      .catch(() => {
        hits = [];
        show("The search index did not load. Check your connection and type again.");
      });
  }

  function remember(text) {
    if (!window.history || !window.history.replaceState) return;
    const url = new URL(window.location.href);
    if (text) url.searchParams.set("q", text);
    else url.searchParams.delete("q");
    window.history.replaceState(null, "", url.toString());
  }

  function go(entry) {
    window.location.href = root + entry.url;
  }

  box.addEventListener("input", search);
  box.addEventListener("keydown", (ev) => {
    if (ev.key === "ArrowDown" || ev.key === "ArrowUp") {
      if (!hits.length) return;
      ev.preventDefault();
      selected = Math.max(0, Math.min(hits.length - 1, selected + (ev.key === "ArrowDown" ? 1 : -1)));
      render();
    } else if (ev.key === "Enter" && hits[selected]) {
      go(hits[selected]);
    } else if (ev.key === "Escape") {
      box.blur();
      close();
    }
  });

  function isTyping(el) {
    return !!el && (el.isContentEditable || el.tagName === "INPUT" || el.tagName === "TEXTAREA");
  }

  document.addEventListener("keydown", (ev) => {
    if (ev.key === "/" && !isTyping(document.activeElement)) {
      ev.preventDefault();
      box.focus();
      box.select();
    }
  });

  document.addEventListener("click", (ev) => {
    if (!panel.contains(ev.target) && ev.target !== box) close();
  });

  const asked = new URL(window.location.href).searchParams.get("q");
  if (asked) {
    box.value = asked;
    search();
  }
})();
