@echo off
rem Builds the in-browser VM on Windows. build_wasm.sh is the same build for Linux,
rem WSL and macOS; a POSIX shell cannot resolve emcmake, which the SDK installs as
rem emcmake.bat.
rem
rem It provisions the SDK itself, so a first build needs no separate step. Needs
rem python3 and cmake on PATH; the SDK ships neither.
rem
rem The cmake tree lands in <quirrel>\build\emscripten, not in this directory. Set
rem WASM_BUILD_DIR to put it somewhere else.
setlocal

rem Nothing to do when the committed pair already describes these sources: no SDK
rem to fetch, no compile. Pass --force after changing VM behaviour without bumping
rem the version, which the stamp cannot see.
set "FORCE="
set "HAVE="
set "WANT="
if /i "%~1"=="--force" set "FORCE=1"
if /i "%~1"=="-f" set "FORCE=1"
if defined FORCE goto :rebuild
if not exist "%~dp0quirrel.js" goto :rebuild
if not exist "%~dp0quirrel.wasm" goto :rebuild
if not exist "%~dp0quirrel.version" goto :rebuild
for /f "usebackq delims=" %%v in ("%~dp0quirrel.version") do set "HAVE=%%v"
for /f %%v in ('python3 "%~dp0..\gen\quirrel_version.py"') do set "WANT=%%v"
if not "%HAVE%"=="%WANT%" goto :rebuild
echo wasm: quirrel.js and quirrel.wasm are already built from %HAVE%; --force rebuilds
exit /b 0
:rebuild

rem Cheap when the pinned version is already active, and the only way a first
rem build works without a separate step.
python3 "%~dp0install_emsdk.py" || exit /b 1
for /f "delims=" %%d in ('python3 "%~dp0install_emsdk.py" --dir') do set "EMSDK=%%d"
if "%EMSDK%"=="" exit /b 1

for %%i in ("%~dp0..\..") do set "QUIRREL=%%~fi"
if "%WASM_BUILD_DIR%"=="" set "WASM_BUILD_DIR=%QUIRREL%\build\emscripten"

call "%EMSDK%\emsdk_env.bat" || exit /b 1
rem `call` on both: emcmake is a .bat, and a .bat invoked without it never returns
rem A tree configured by the other host records paths this one cannot use, so a
rem failed configure is retried once from scratch rather than reported.
call emcmake cmake -G Ninja -S "%~dp0." -B "%WASM_BUILD_DIR%" || (
  echo wasm: reconfiguring %WASM_BUILD_DIR% from scratch
  rmdir /s /q "%WASM_BUILD_DIR%"
  call emcmake cmake -G Ninja -S "%~dp0." -B "%WASM_BUILD_DIR%" || exit /b 1
)
call cmake --build "%WASM_BUILD_DIR%" --parallel || exit /b 1

rem The version the artifacts describe, so build_rtd.sh can tell whether the
rem committed pair still matches the source it is published beside.
for /f %%v in ('python3 "%~dp0..\gen\quirrel_version.py"') do @echo %%v> "%~dp0quirrel.version"
type "%~dp0quirrel.version"
