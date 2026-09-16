(function () {
  const ESCAPES = { "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#x27;" };

  function escape(text) {
    return text.replace(/[&<>"']/g, (c) => ESCAPES[c]);
  }

  function highlight(code) {
    const lexer = window.QUIRREL_LEXER;
    const token = new RegExp(lexer.pattern, "gs");
    const keywords = new Set(lexer.keywords);
    const out = [];
    let pos = 0;
    let m;
    while ((m = token.exec(code)) !== null) {
      out.push(escape(code.slice(pos, m.index)));
      const text = escape(m[0]);
      const groups = m.groups;
      if (groups.name !== undefined) {
        out.push(keywords.has(m[0]) ? `<span class="k">${text}</span>` : text);
      } else {
        const kind = groups.comment !== undefined ? "c" : groups.string !== undefined ? "s" : "n";
        out.push(`<span class="${kind}">${text}</span>`);
      }
      pos = m.index + m[0].length;
    }
    out.push(escape(code.slice(pos)));
    return out.join("");
  }

  function caretOffset(root) {
    const sel = window.getSelection();
    if (!sel || !sel.rangeCount) return null;
    const range = sel.getRangeAt(0);
    if (!root.contains(range.endContainer)) return null;
    const upto = range.cloneRange();
    upto.selectNodeContents(root);
    upto.setEnd(range.endContainer, range.endOffset);
    return upto.toString().length;
  }

  function setCaret(root, offset) {
    const walker = document.createTreeWalker(root, NodeFilter.SHOW_TEXT);
    const range = document.createRange();
    let seen = 0;
    let node;
    let placed = false;
    while ((node = walker.nextNode()) !== null) {
      const end = seen + node.nodeValue.length;
      if (offset <= end) {
        range.setStart(node, offset - seen);
        placed = true;
        break;
      }
      seen = end;
    }
    if (!placed) {

      range.selectNodeContents(root);
      range.collapse(false);
    } else {
      range.collapse(true);
    }
    const sel = window.getSelection();
    sel.removeAllRanges();
    sel.addRange(range);
  }

  function recolour(code) {
    if (code.dataset.composing === "1") return;
    const offset = caretOffset(code);
    const marked = highlight(code.textContent);
    if (marked === code.innerHTML) return;
    code.innerHTML = marked;
    if (offset !== null) setCaret(code, offset);
  }

  const DEPTH = 200;

  function makeEditable(code) {

    code.contentEditable = "plaintext-only";
    if (code.contentEditable !== "plaintext-only") code.contentEditable = "true";
    code.spellcheck = false;

    const past = [];
    const future = [];
    let current = { text: code.textContent, caret: null };

    function restore(state) {
      code.textContent = state.text;
      recolour(code);
      setCaret(code, state.caret === null ? state.text.length : state.caret);
    }

    code.addEventListener("input", () => {
      past.push(current);
      if (past.length > DEPTH) past.shift();
      future.length = 0;
      recolour(code);
      current = { text: code.textContent, caret: caretOffset(code) };
    });

    code.addEventListener("keydown", (ev) => {
      const undo = (ev.ctrlKey || ev.metaKey) && ev.key.toLowerCase() === "z";
      const redo = ((ev.ctrlKey || ev.metaKey) && ev.key.toLowerCase() === "y") ||
                   ((ev.ctrlKey || ev.metaKey) && ev.shiftKey && ev.key.toLowerCase() === "z");
      if (!undo && !redo) return;
      ev.preventDefault();
      if (redo || ev.shiftKey) {
        const next = future.pop();
        if (!next) return;
        past.push(current);
        current = next;
      } else {
        const prev = past.pop();
        if (!prev) return;
        future.push(current);
        current = prev;
      }
      restore(current);
    });

    code.addEventListener("compositionstart", () => { code.dataset.composing = "1"; });
    code.addEventListener("compositionend", () => {
      code.dataset.composing = "0";
      recolour(code);
      current = { text: code.textContent, caret: caretOffset(code) };
    });
  }

  window.quirrelEditor = { highlight, recolour, makeEditable };
})();
