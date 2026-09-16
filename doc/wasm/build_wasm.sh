#!/bin/sh
# Builds the in-browser VM used by the "Run this code" button.
#
#     wasm/build_wasm.sh [--force]
#
# It provisions and activates emscripten itself, so nothing has to be installed or
# sourced first: install_emsdk.py picks the SDK directory ($EMSDK, $GDEVTOOL/emsdk
# or ~/.emsdk) and installs the pinned version when it is not there.
#
# Writes quirrel.js and quirrel.wasm next to this script. build.py copies them into
# the site when they are there; without them the site still builds and the Run
# button says the VM is missing.
#
# The cmake tree lands in <quirrel>/build/emscripten, not in this directory.
# Overrides: WASM_BUILD_DIR, WASM_GENERATOR (default Ninja, from the SDK),
# WASM_JOBS, and EM_CACHE when the SDK itself is read-only.
set -e

HERE=$(cd "$(dirname "$0")" && pwd)

force=
case "${1:-}" in
  -f|--force) force=1 ;;
  "") ;;
  *) echo "build_wasm.sh: unknown argument $1 (only --force)" >&2; exit 1 ;;
esac

# Nothing to do when the committed pair already describes these sources: no SDK to
# fetch, no compile. Pass --force after changing VM behaviour without bumping the
# version, which the stamp cannot see.
if [ -z "$force" ]; then
  want=$(python3 "$HERE/../gen/quirrel_version.py")
  have=$(tr -d "\r" < "$HERE/quirrel.version" 2>/dev/null || echo none)
  if [ "$want" = "$have" ] && [ -f "$HERE/quirrel.js" ] && [ -f "$HERE/quirrel.wasm" ]; then
    echo "wasm: quirrel.js and quirrel.wasm are already built from $have; --force rebuilds"
    exit 0
  fi
fi

# Cheap when the pin is already active, and the only way a first build works
# without a separate step.
python3 "$HERE/install_emsdk.py" || exit 1
EMSDK=$(python3 "$HERE/install_emsdk.py" --dir) || exit 1

# `emsdk construct_env` is the whole of what emsdk_env.sh evaluates. Calling it
# directly skips that script's guess at its own location, which is the part that
# fails in a shell it does not recognize.
if ! command -v emcmake >/dev/null 2>&1; then
  eval "$(EMSDK_BASH=1 "$EMSDK/emsdk" construct_env)"
fi
command -v emcmake >/dev/null 2>&1 || {
  echo "build_wasm.sh: emcmake is not on PATH even after activating $EMSDK." >&2
  echo "  On Windows use wasm/build_wasm.bat: the SDK installs emcmake as a .bat," >&2
  echo "  which a POSIX shell does not resolve." >&2
  exit 1
}

# The cmake tree goes under the library's build directory, not beside the pages.
BUILD=${WASM_BUILD_DIR:-$(cd "$HERE/../.." && pwd)/build/emscripten}
# A cache configured on another machine cannot be reused: the compiler it recorded
# is not there. Start over rather than fail with cmake's version of that.
if [ -f "$BUILD/CMakeCache.txt" ]; then
  cxx=$(sed -n 's/^CMAKE_CXX_COMPILER:[^=]*=//p' "$BUILD/CMakeCache.txt")
  if [ -z "$cxx" ] || [ ! -f "$cxx" ]; then
    echo "build_wasm.sh: $BUILD was configured elsewhere ($cxx); starting over"
    rm -rf "$BUILD"
  fi
fi

emcmake cmake -G "${WASM_GENERATOR:-Ninja}" -S "$HERE" -B "$BUILD"
cmake --build "$BUILD" --parallel "${WASM_JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)}"

# The version the artifacts describe. They are committed, so build_rtd.sh compares
# this against the header to tell whether the pair still matches its own sources.
python3 "$HERE/../gen/quirrel_version.py" > "$HERE/quirrel.version"

ls -l "$HERE/quirrel.js" "$HERE/quirrel.wasm" "$HERE/quirrel.version"
