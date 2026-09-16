"""Provisions the emscripten SDK the doc site's in-browser VM is built with.

    python3 wasm/install_emsdk.py          # into $EMSDK, $GDEVTOOL/emsdk or ~/.emsdk
    python3 wasm/install_emsdk.py --dir    # only prints that directory

It is a no-op, and touches no network, when the pinned version is already active,
so the build scripts call it every time and a devtools image that already carries
the SDK is left alone. The build scripts read the SDK directory from --dir.

One implementation for both hosts: the same checks in sh and in batch were two
things to keep in step, and the batch half could not quote its way through a path.
install_emsdk.sh and install_emsdk.bat are wrappers around this.

Needs git, and https to github.com (the emsdk repository) and to
storage.googleapis.com (the toolchain, node and ninja).
"""

import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PIN = os.path.join(HERE, "emsdk_version.sh")


def pinned():
    """The versions emsdk_version.sh names. It is shell so the sh half can source
    it, but the assignments are plain enough to read from here."""
    want = {"EMSDK_EMSCRIPTEN_VERSION": None, "EMSDK_NINJA_VERSION": None}
    with open(PIN, encoding="utf-8") as f:
        for line in f:
            key, _, value = line.strip().partition("=")
            if key in want:
                want[key] = value.strip().strip("'\"")
    missing = [k for k, v in want.items() if not v]
    if missing:
        raise SystemExit(f"{PIN}: no {' or '.join(missing)}")
    return want["EMSDK_EMSCRIPTEN_VERSION"], want["EMSDK_NINJA_VERSION"]


DRIVE = re.compile(r"^[A-Za-z]:[/\\]")


def to_posix(path):
    """A Windows EMSDK or GDEVTOOL read by a POSIX python.

    MSYS mounts the drive at /d and WSL at /mnt/d, so there is no rewrite that
    serves both: ask the shell that knows. Guessing would join the raw value and
    make a directory named after the drive, in the working directory.
    """
    if os.name == "nt" or not DRIVE.match(path):
        return path
    for tool in ("cygpath", "wslpath"):
        try:
            done = subprocess.run([tool, "-u", path], capture_output=True, text=True)
        except OSError:
            continue
        if done.returncode == 0 and done.stdout.strip():
            return done.stdout.strip()
    raise SystemExit(f"install_emsdk: {path} is a Windows path, this is a POSIX "
                     "python, and neither cygpath nor wslpath is here to translate "
                     "it. Set EMSDK or GDEVTOOL to a path this shell can open.")


def sdk_dir():
    """An activated SDK wins, then the Dagor devtools root, then ~/.emsdk."""
    if os.environ.get("EMSDK"):
        return to_posix(os.environ["EMSDK"])
    devtools = (os.environ.get("GDEVTOOL") or "").strip().strip('"')
    if devtools:
        return os.path.join(to_posix(devtools), "emsdk")
    return os.path.join(os.path.expanduser("~"), ".emsdk")


def active(sdk, emscripten, ninja):
    """The pinned emscripten, an activated config, a linker, and the generator the
    build asks cmake for."""
    config = os.path.join(sdk, ".emscripten")
    if not os.path.isfile(config):
        return False
    binaries = os.path.join(sdk, "upstream", "bin")
    if not any(os.path.isfile(os.path.join(binaries, n))
               for n in ("wasm-opt", "wasm-opt.exe")):
        return False
    stamp = os.path.join(sdk, "upstream", "emscripten", "emscripten-version.txt")
    try:
        with open(stamp, encoding="utf-8") as f:
            if f.read().strip().strip('"') != emscripten:
                return False
    except OSError:
        return False
    with open(config, encoding="utf-8") as f:
        return f"ninja/{ninja}" in f.read()


def run(cmd, **kw):
    print("+ " + " ".join(cmd), flush=True)
    return subprocess.call(cmd, **kw)


def main():
    sdk = sdk_dir()
    if sys.argv[1:] == ["--dir"]:
        print(sdk)
        return 0
    if sys.argv[1:]:
        raise SystemExit(f"install_emsdk: unknown arguments {sys.argv[1:]} (only --dir)")
    emscripten, ninja = pinned()
    if active(sdk, emscripten, ninja):
        print(f"emsdk: emscripten {emscripten} is already active in {sdk}")
        return 0

    os.makedirs(os.path.dirname(sdk) or ".", exist_ok=True)
    if not os.path.isdir(os.path.join(sdk, ".git")):
        if run(["git", "clone", "--depth", "1",
                "https://github.com/emscripten-core/emsdk.git", sdk]):
            raise SystemExit("install_emsdk: could not clone the emsdk repository")

    # .bat on Windows, an extensionless script elsewhere.
    emsdk = os.path.join(sdk, "emsdk.bat" if os.name == "nt" else "emsdk")
    tools = [emscripten, f"ninja-{ninja}-64bit"]
    # An existing checkout is tried as it is first: it usually knows the pinned
    # version already, and updating it is rude to a hand-made clone.
    if run([emsdk, "install"] + tools, cwd=sdk):
        print(f"emsdk: the checkout does not know {emscripten}, updating it")
        if run(["git", "-C", sdk, "fetch", "--depth", "1", "origin", "HEAD"]) or \
           run(["git", "-C", sdk, "checkout", "-f", "FETCH_HEAD"]) or \
           run([emsdk, "install"] + tools, cwd=sdk):
            raise SystemExit(f"install_emsdk: could not install {emscripten}")
    if run([emsdk, "activate"] + tools, cwd=sdk):
        raise SystemExit(f"install_emsdk: could not activate {emscripten}")

    if not active(sdk, emscripten, ninja):
        raise SystemExit(f"install_emsdk: {sdk} is still not usable after activate")
    print(f"emsdk: emscripten {emscripten} installed in {sdk}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
