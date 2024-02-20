import os
import sys

PROJ_ROOT_PATH = os.path.realpath(os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..', '..')))
THIRD_PARTY_PATH = os.path.join(PROJ_ROOT_PATH, 'third_party')

PICO_INSTALL_PATH = os.path.join(THIRD_PARTY_PATH, 'pico-dist')
PICO_SDK_PATH = os.path.join(PICO_INSTALL_PATH, 'pico-sdk')
PICO_TOOLCHAIN_PATH = os.path.join(PICO_INSTALL_PATH, 'gcc-arm-none-eabi')

INTERMEDIATE_DIR = '_intermediate'
CMAKE_OUTPUT_PATH = os.path.join(PROJ_ROOT_PATH, INTERMEDIATE_DIR)

CMAKE_EXE = os.path.join(PICO_INSTALL_PATH, 'cmake', 'bin', 'cmake.exe')
NINJA_EXE = os.path.join(PICO_INSTALL_PATH, 'ninja', 'ninja.exe')

OPENOCD_PATH = os.path.join(PICO_INSTALL_PATH, 'openocd')
OPENOCD_SCRIPTS_PATH = os.path.join(OPENOCD_PATH, 'scripts')
OPENOCD_EXE = os.path.join(OPENOCD_PATH, 'openocd.exe')

PYTHON_PATH = os.path.join(PICO_INSTALL_PATH, 'python')
PYTHON_EXE = os.path.join(PYTHON_PATH, 'python.exe')

EXTRA_CMAKE_ARGS = [
    # '--trace-expand',
    '-DCMAKE_EXPORT_COMPILE_COMMANDS=1'
]

EXTRA_ENV_VARS = {
    'PICO_SDK_PATH': PICO_SDK_PATH,
    'PICO_INSTALL_PATH': PICO_INSTALL_PATH,
    'PICO_TOOLCHAIN_PATH': PICO_TOOLCHAIN_PATH,

    # Extending PATH with additional executables bundled with Pico SDK
    'PATH': ';'.join([
        os.path.join(PICO_TOOLCHAIN_PATH, 'bin'),
        os.path.join(PICO_INSTALL_PATH, 'ninja'),
        os.path.join(PICO_INSTALL_PATH, 'python'),
        os.environ['PATH']
    ]),
}

if sys.platform.startswith('win'):
    # CMake cannot find Python without this on Windows
    EXTRA_ENV_VARS['SYSTEMROOT'] = os.environ['SYSTEMROOT']
