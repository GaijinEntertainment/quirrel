"""Prints the Quirrel version the sources in this tree carry.

    python gen/quirrel_version.py     # 4.38.0

The header is the only place the number lives, so the doc build, the wasm build and
the Read the Docs driver all read it from here rather than each parsing the header
their own way.
"""

import os
import re
import sys

HEADER = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                       "..", "..", "include", "squirrel.h"))
_PART = re.compile(r"^#define SQUIRREL_VERSION_NUMBER_(MAJOR|MINOR|PATCH)\s+(\d+)", re.M)


def version():
    """major.minor.patch, or "" when the header cannot be read.

    Empty rather than an exception: a caller stamping a page header or comparing a
    build artifact has something sensible to do without it.
    """
    try:
        with open(HEADER, encoding="utf-8", errors="replace") as f:
            parts = dict(_PART.findall(f.read()))
    except OSError:
        return ""
    try:
        return ".".join(parts[k] for k in ("MAJOR", "MINOR", "PATCH"))
    except KeyError:
        return ""


if __name__ == "__main__":
    v = version()
    if not v:
        print(f"cannot read the version from {HEADER}", file=sys.stderr)
        raise SystemExit(1)
    print(v)
