"""Builds the Quirrel reference site.

    python doc/build.py [--out doc/_site] [--version version] [--sq path]

The build makes the interpreter with cmake, or takes the one --sq names, then runs
gen/dump_api.nut to refresh gen/api_dump.jsonl. Both steps are advisory: a machine
with no toolchain warns and keeps the committed dump, so the site is always
published. The side-cars under content/ and the examples are committed.
"""

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "gen"))

import model            # noqa: E402
import render           # noqa: E402

API_DUMP = os.path.join(HERE, "gen", "api_dump.jsonl")
DUMP_API = os.path.join(HERE, "gen", "dump_api.nut")


def build_sq():
    try:
        subprocess.run(["cmake", "-S", model.QUIRREL, "-B", model.CMAKE_BUILD,
                        "-DCMAKE_BUILD_TYPE=Release"], check=True)
        subprocess.run(["cmake", "--build", model.CMAKE_BUILD, "--config", "Release",
                        "--target", "sq", "--parallel"], check=True)
        return True
    except (OSError, subprocess.SubprocessError) as err:
        print(f"WARNING: the interpreter was not built ({err}), so the committed", file=sys.stderr)
        print("WARNING: gen/api_dump.jsonl stands and may not match the sources", file=sys.stderr)
        return False


def refresh_api_dump(sq):
    dump_tmp = None
    try:
        with tempfile.NamedTemporaryFile(dir=os.path.dirname(API_DUMP), delete=False) as f:
            dump_tmp = f.name
            subprocess.run([sq, DUMP_API], stdout=f, check=True)
        os.replace(dump_tmp, API_DUMP)
        dump_tmp = None
    except (OSError, subprocess.SubprocessError) as err:
        print(f"WARNING: gen/dump_api.nut did not run ({err}), so the committed", file=sys.stderr)
        print("WARNING: gen/api_dump.jsonl stands and may not match the sources", file=sys.stderr)
    finally:
        if dump_tmp:
            try:
                os.unlink(dump_tmp)
            except OSError:
                pass


def write(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)


def sq_version(sq):
    """The interpreter's version, which is the version the pages describe.

    `sq -v` prints the number, then the copyright, on one line; the number is
    everything before the first space. A missing or unrunnable interpreter is not an
    error: the site is built from committed files and only loses the label.
    """
    try:
        done = subprocess.run([sq, "-v"], capture_output=True, text=True, timeout=30)
    except (OSError, subprocess.SubprocessError):
        return ""
    # It prints to stderr on some builds and stdout on others; take whichever spoke.
    line = (done.stdout or done.stderr).strip().splitlines()
    if not line:
        return ""
    version = line[0].split(" ")[0].strip()
    # Guard against a usage message or a banner: a version starts with a digit.
    return version if version[:1].isdigit() else ""


def clean(out_dir):
    """Empties a previous build, so a page that no longer exists cannot survive.

    Only a directory that looks like one of ours is removed: an --out pointing
    somewhere unexpected is left alone rather than deleted.
    """
    if not os.path.isdir(out_dir):
        return
    ours = all(os.path.exists(os.path.join(out_dir, name))
               for name in ("index.html", "search.json"))
    if not ours:
        raise SystemExit(f"{out_dir} exists but is not a previous build "
                         "(no index.html and search.json); refusing to empty it")
    shutil.rmtree(out_dir)


def capi_entries(site):
    """(row, url) for every C API function that some page lists.

    A function whose section no page shows has no anchor to point at, so it is left
    out rather than indexed with a link that goes nowhere; gen/check.py reports the
    section instead.
    """
    pages = render.capi_group_pages(site)
    for row in site.capi:
        url = pages.get(row["group"])
        if url:
            yield row, f"{url}#{row['name']}"


def build(out_dir, version="", committed_api_dump=False, sq=None):
    # An interpreter the caller names is theirs to build; only the cmake one is ours.
    refresh = not committed_api_dump and (model.named_sq(sq) or build_sq())
    sq = model.find_sq(sq)      # after build_sq, so a fresh cmake output is found
    if refresh:
        refresh_api_dump(sq)
    site = model.load()
    site.version = version or sq_version(sq)
    missing_refs = []
    clean(out_dir)

    for container in site.containers:
        if container.inlined:
            continue        # the parent's page carries it, so it has none of its own
        write(os.path.join(out_dir, *container.url.split("/")),
              render.render_container(site, container, missing_refs))

    for symbol in site.symbols:
        write(os.path.join(out_dir, *symbol.url.split("/")),
              render.render_symbol(site, symbol, missing_refs))

    for page in site.pages:
        write(os.path.join(out_dir, page.url), render.render_page(site, page, missing_refs))

    write(os.path.join(out_dir, "index.html"), render.render_index(site, missing_refs))

    index = [
        {
            "kind": "symbol",
            "ref": s.ref,
            "name": s.name,
            "url": s.url,
            "sig": s.decl or "",
            "doc": s.summary,
            "container": s.container.title,
        }
        for s in site.symbols
    ]

    # Sections of a symbol page: "math.clamp > Errors" jumps straight to the anchor.
    for s in site.symbols:
        if not s.documented:
            continue
        for heading, _text in s.body.sections:
            if heading:
                index.append({
                    "kind": "section",
                    "ref": f"{s.ref} > {heading}",
                    "name": heading,
                    "url": f"{s.url}#{model.anchor(heading)}",
                    "sig": "",
                    "doc": s.container.title,
                })

    for c in site.containers:
        kind = "module" if c.kind in ("module", "root") else "class"
        index.append({"kind": kind, "ref": c.title, "name": c.title, "url": c.link_url,
                      "sig": "", "doc": c.blurb})

    for page in site.pages:
        index.append({"kind": "page", "ref": page.title, "name": page.title,
                      "url": page.url, "sig": "", "doc": page.group})
        for heading, anchor in page.sections:
            index.append({"kind": "section", "ref": f"{page.title} > {heading}",
                          "name": heading, "url": f"{page.url}#{anchor}",
                          "sig": "", "doc": page.title})

    # A keyword is what someone actually types, so it gets its own entry pointing at
    # the section that explains it.
    for word, target in sorted(site.keywords.items()):
        page_name, _, anchor = target.partition("#")
        url = site.page_urls[page_name] + (f"#{anchor}" if anchor else "")
        index.append({"kind": "keyword", "ref": word, "name": word, "url": url,
                      "sig": "keyword", "doc": "Quirrel keyword"})
    for group in site.operators:
        for entry in group["operators"]:
            page_name, _, anc = entry["target"].partition("#")
            url = site.page_urls[page_name] + (f"#{anc}" if anc else "")
            index.append({"kind": "operator", "ref": entry["op"], "name": entry["op"],
                          "url": url, "sig": entry.get("mm", ""), "doc": entry["what"]})

    # A metamethod is typed as often as a keyword is, and `_cmp` matches nothing in
    # the symbol list, so it needs an entry of its own.
    for name, entry in site.metamethods.items():
        page_name, _, anc = entry["target"].partition("#")
        url = site.page_urls[page_name] + (f"#{anc}" if anc else "")
        index.append({"kind": "metamethod", "ref": name, "name": name, "url": url,
                      "sig": entry["sig"], "doc": f"runs when {entry['when']}"})

    for row, url in capi_entries(site):
        index.append({"kind": "native", "ref": row["qualified"], "name": row["name"],
                      "url": url, "sig": render.capi_declaration(row),
                      "doc": site.capi_docs.get(row["name"], "")})

    write(os.path.join(out_dir, "search.json"), json.dumps(index, separators=(",", ":")))

    for name in ("style.css", "theme.js", "search.js", "nav.js", "run.js", "run_worker.js", "highlight.js"):
        shutil.copyfile(os.path.join(HERE, "theme", name), os.path.join(out_dir, name))

    # The browser highlighter recolours an edited sample, and it has to agree with
    # the one that coloured the page. Both read this.
    import markdown  # noqa: E402
    write(os.path.join(out_dir, "lexer.js"),
          "window.QUIRREL_LEXER = " + json.dumps(markdown.js_lexer(), separators=(",", ":")) + ";\n")

    # The VM is an optional part of the site: wasm/build_wasm.sh needs emscripten,
    # which the doc build does not. Without it the Run button reports itself
    # unavailable and every page is otherwise complete.
    vm = [f for f in ("quirrel.js", "quirrel.wasm")
          if os.path.exists(os.path.join(HERE, "wasm", f))]
    for name in vm:
        shutil.copyfile(os.path.join(HERE, "wasm", name), os.path.join(out_dir, name))

    if missing_refs:
        for where, ref in missing_refs:
            print(f"error: {where} links to unknown symbol {ref!r}", file=sys.stderr)
        raise SystemExit(f"{len(missing_refs)} unresolved cross-references")

    # The model can only vouch for the links an author wrote. This reads the output
    # back and follows every link in it, anchors included, which is what catches a
    # page that moved into a section of another one.
    import check_links  # noqa: E402
    broken = check_links.check_site(out_dir)
    if broken:
        for err in broken:
            print(f"error: {err}", file=sys.stderr)
        raise SystemExit(f"{len(broken)} broken links")

    documented = sum(1 for s in site.symbols if s.documented)
    with_example = sum(1 for s in site.symbols if s.example)
    print(f"{len(site.containers)} containers, {len(site.symbols)} symbols "
          f"({documented} documented, {with_example} with an example) -> {out_dir}")
    print("in-browser VM: " + ("included" if len(vm) == 2 else
                               "not built, run wasm/build_wasm.sh to add it"))
    print("version: " + (site.version or
                         "not stamped, build the interpreter to read it from there"))




def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", default=os.path.join(HERE, "_site"))
    parser.add_argument("--version", default="",
                        help="the version to stamp")
    parser.add_argument("--committed-api-dump", action="store_true",
                        help="do not build the interpreter or refresh gen/api_dump.jsonl")
    parser.add_argument("--sq", default=None,
                        help="an already built interpreter (default: $QUIRREL_SQ, else cmake)")
    args = parser.parse_args()
    build(args.out, args.version, args.committed_api_dump, args.sq)


if __name__ == "__main__":
    main()
