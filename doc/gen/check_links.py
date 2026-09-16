"""Checks every internal link of a built site, target file and anchor alike.

The generator already fails on an unresolved `sym:` or `page:` reference, but a
link it builds itself - a container that moved into another page, a heading whose
text changed - can still point at nothing. This walks the output instead of the
model, so it sees exactly what a reader clicks.
"""

import html
import os
import re
import sys

HREF = re.compile(r'<a\b[^>]*?href="([^"]+)"', re.I)
ID = re.compile(r'\bid="([^"]+)"')


def _targets(text):
    return {html.unescape(i) for i in ID.findall(text)}


def check_site(out_dir):
    """Returns a list of "file: link" problems, empty when every link resolves."""
    files = {}
    for dirpath, _dirs, names in os.walk(out_dir):
        for name in names:
            if name.endswith(".html"):
                path = os.path.join(dirpath, name)
                rel = os.path.relpath(path, out_dir).replace(os.sep, "/")
                with open(path, encoding="utf-8") as f:
                    files[rel] = f.read()

    ids = {rel: _targets(text) for rel, text in files.items()}
    errors = []
    for rel, text in sorted(files.items()):
        base = os.path.dirname(rel)
        # A page links the same target from its trail, its body and its See also, and
        # one line per broken target is enough to fix it.
        for href in dict.fromkeys(HREF.findall(text)):
            href = html.unescape(href)
            if href.startswith(("http://", "https://", "mailto:", "//")):
                continue
            target, _, anchor = href.partition("#")
            if not target:
                target = rel                        # a link inside the page
            else:
                target = os.path.normpath(os.path.join(base, target)).replace(os.sep, "/")
                if target not in files:
                    if not os.path.exists(os.path.join(out_dir, *target.split("/"))):
                        errors.append(f"{rel}: {href} -> no such file")
                    continue                        # a non-html asset has no anchors
            if anchor and anchor not in ids.get(target, set()):
                errors.append(f"{rel}: {href} -> no id {anchor!r} in {target}")
    return errors


def main():
    out_dir = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "_site")
    errors = check_site(out_dir)
    for err in errors:
        print(f"error: {err}", file=sys.stderr)
    if errors:
        raise SystemExit(f"{len(errors)} broken links")
    print("all internal links resolve")


if __name__ == "__main__":
    main()
