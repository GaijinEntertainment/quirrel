@echo off
rem Provisions the emscripten SDK the doc site's in-browser VM is built with.
rem
rem     wasm\install_emsdk.bat
rem
rem A no-op that touches no network when the pinned version is already active, so
rem build_wasm.bat calls it every time. install_emsdk.py holds the logic, one copy
rem for both hosts; this is here so Windows has the name it expects.
python3 "%~dp0install_emsdk.py" %*
