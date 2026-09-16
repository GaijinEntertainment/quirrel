#!/bin/sh
# Provisions the emscripten SDK the doc site's in-browser VM is built with.
#
#     wasm/install_emsdk.sh                  # into $EMSDK, $GDEVTOOL/emsdk or ~/.emsdk
#     EMSDK=/opt/emsdk wasm/install_emsdk.sh
#
# A no-op that touches no network when the pinned version is already active, so the
# build scripts call it every time. install_emsdk.py holds the logic, one copy for
# both hosts; this is here so the documented name keeps working.
exec python3 "$(cd "$(dirname "$0")" && pwd)/install_emsdk.py" "$@"
