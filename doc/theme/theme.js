(function () {
  const KEY = "quirrel:theme";
  const root = document.documentElement;
  const controls = Array.from(document.querySelectorAll("[data-theme-control]"));
  const themes = new Set(controls.map((control) => control.value));
  const dark = window.matchMedia("(prefers-color-scheme: dark)");

  function systemTheme() {
    return dark.matches ? "dark" : "light";
  }

  function setTheme(theme) {
    root.dataset.theme = theme;
    controls.forEach((control) => {
      control.checked = control.value === theme;
    });
  }

  let saved = null;
  try {
    saved = window.localStorage.getItem(KEY);
  } catch (e) {}
  if (!themes.has(saved)) saved = null;
  setTheme(saved || systemTheme());

  controls.forEach((control) => {
    control.addEventListener("change", () => {
      if (!control.checked) return;
      const theme = control.value;
      saved = theme;
      setTheme(theme);
      try {
        window.localStorage.setItem(KEY, theme);
      } catch (e) {}
    });
  });

  dark.addEventListener("change", () => {
    if (!saved) setTheme(systemTheme());
  });
})();
