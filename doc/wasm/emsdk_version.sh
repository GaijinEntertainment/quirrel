# The emscripten SDK the in-browser VM is built with. install_emsdk.py installs and
# activates exactly this when the SDK it finds is a different one, so the VM one
# machine builds is the VM another one builds.
#
# To bump: edit the numbers, run install_emsdk.sh, rebuild, and check the VM still
# agrees with the committed example output (node check_examples.js) before
# committing.
EMSDK_EMSCRIPTEN_VERSION=6.0.6
EMSDK_NINJA_VERSION=1.13.2
