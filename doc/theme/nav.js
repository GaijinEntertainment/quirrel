(function () {
  const KEY = "quirrel:sidebar-scroll";
  const sidebar = document.getElementById("sidebar");
  if (!sidebar) return;

  let store = null;
  try {
    store = window.sessionStorage;
  } catch (e) {
    store = null;
  }

  const saved = store && store.getItem(KEY);
  if (saved !== null && saved !== undefined) sidebar.scrollTop = Number(saved) || 0;

  const here = sidebar.querySelector("a.here");
  if (here) {
    const link = here.getBoundingClientRect();
    const box = sidebar.getBoundingClientRect();
    if (link.top < box.top || link.bottom > box.bottom) {
      sidebar.scrollTop += link.top - box.top - (box.height - link.height) / 2;
    }
  }

  const button = document.getElementById("menu");
  const scrim = document.querySelector(".scrim");
  const main = document.getElementById("main");

  function narrow() {
    return window.matchMedia("(max-width: 54em)").matches;
  }

  function setOpen(open, focus) {
    document.body.classList.toggle("menu-open", open);
    if (button) button.setAttribute("aria-expanded", open ? "true" : "false");
    if (scrim) scrim.hidden = !open;
    if (!focus) return;
    if (open) {
      const first = sidebar.querySelector("a.here") || sidebar.querySelector("a");
      if (first) first.focus();
    } else if (button) {
      button.focus();
    }
  }

  if (button) {
    button.addEventListener("click", () => setOpen(!document.body.classList.contains("menu-open"), true));
  }

  if (scrim) scrim.addEventListener("click", () => setOpen(false, true));

  sidebar.addEventListener("click", (ev) => {
    if (!narrow() || !ev.target.closest("a")) return;
    setOpen(false, false);
    if (main) main.focus({ preventScroll: true });
  });

  document.addEventListener("keydown", (ev) => {
    if (ev.key !== "Escape" || !document.body.classList.contains("menu-open")) return;
    ev.preventDefault();
    setOpen(false, true);
  });

  document.addEventListener("focusin", (ev) => {
    if (!narrow() || !document.body.classList.contains("menu-open")) return;
    if (sidebar.contains(ev.target) || (button && button === ev.target)) return;
    setOpen(false, false);
  });

  window.addEventListener("resize", () => {
    if (!narrow()) setOpen(false, false);
  });

  if (!store) return;

  let pending = 0;
  sidebar.addEventListener("scroll", () => {
    if (pending) return;
    pending = setTimeout(() => {
      pending = 0;
      try {
        store.setItem(KEY, String(Math.round(sidebar.scrollTop)));
      } catch (e) {}
    }, 120);
  });
})();
