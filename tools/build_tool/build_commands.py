import os
import shutil
import subprocess
import sys

from build_tool.constants import *


def noop(args):
    pass


def clean(args):
    try:
        if os.path.exists(CMAKE_OUTPUT_PATH):
            shutil.rmtree(CMAKE_OUTPUT_PATH)
            print(f'directory was deleted: {CMAKE_OUTPUT_PATH}')
        else:
            print('nothing to clean')
    except OSError as e:
        print('error:', e, file=sys.stderr)
        exit(1)


def convert_pd(args):
    import hvcc

    program_path = os.path.join(PROJ_ROOT_PATH, 'programs', args.program)
    pd_patch_path = os.path.join(program_path, 'puredata', '_main.pd')
    if not os.path.exists(pd_patch_path):
        print('error: could not find Pure Data patch', pd_patch_path, file=sys.stderr)
        exit(1)
    hv_out_path = os.path.join(program_path, 'hvcc_gen')
    try:
        if os.path.exists(hv_out_path):
            shutil.rmtree(hv_out_path)
    except OSError as e:
        print(f'error: could not delete directory {hv_out_path}', file=sys.stderr)
        print(e, file=sys.stderr)
        exit(1)
    hvcc.compile_dataflow(pd_patch_path, hv_out_path, 'PdPatch')
    print(f"success: you may need to run the 'configure' command to let CMake know about new sources files")


def configure(args):
    cmd = [CMAKE_EXE, '-S', '.', '-B', INTERMEDIATE_DIR, '-G', 'Ninja'] + EXTRA_CMAKE_ARGS
    subprocess.run(cmd, cwd=PROJ_ROOT_PATH, env=EXTRA_ENV_VARS)


def build(args):
    if args.program == 'all':
        cmd = [CMAKE_EXE, '--build', INTERMEDIATE_DIR] + EXTRA_CMAKE_ARGS
        subprocess.run(cmd, cwd=PROJ_ROOT_PATH)
    else:
        cmd = [NINJA_EXE, args.program]
        subprocess.run(cmd, cwd=CMAKE_OUTPUT_PATH)


def deploy_swd(args):
    firmware_dir = os.path.join(CMAKE_OUTPUT_PATH, 'programs', args.program)
    firmware_filename = f'{args.program}.elf'
    firmware_path = os.path.join(firmware_dir, firmware_filename)
    if not os.path.exists(firmware_path):
        print('error: firmware file does not exist', firmware_path, file=sys.stderr)
        exit(1)

    cmd = [OPENOCD_EXE,
           '-s', OPENOCD_SCRIPTS_PATH.replace('\\', '/'),
           '-f', 'interface/cmsis-dap.cfg',
           '-f', 'target/rp2040.cfg',
           '-c', '"adapter speed 5000"',
           '-c', f'"program {firmware_filename} verify reset exit"',
           ]
    cmd_concatenated = ' '.join(cmd)  # for some reason, it's important for OpenOCD
    subprocess.run(cmd_concatenated, cwd=firmware_dir)


def pico_sdk(args):
    if args.action == 'configure_examples':
        cmd = [CMAKE_EXE, '-S', '.', '-B', INTERMEDIATE_DIR, '-G', 'Ninja'] + EXTRA_CMAKE_ARGS
        cwd = os.path.join(THIRD_PARTY_PATH, 'pico-examples')
        subprocess.run(cmd, cwd=cwd, env=EXTRA_ENV_VARS)
    elif args.action == 'build_examples':
        cmd = [CMAKE_EXE, '--build', INTERMEDIATE_DIR] + EXTRA_CMAKE_ARGS
        cwd = os.path.join(THIRD_PARTY_PATH, 'pico-examples')
        subprocess.run(cmd, cwd=cwd)
    elif args.action == 'configure_extras':
        cmd = [CMAKE_EXE, '-S', '.', '-B', INTERMEDIATE_DIR, '-G', 'Ninja'] + EXTRA_CMAKE_ARGS
        cwd = os.path.join(THIRD_PARTY_PATH, 'pico-extras')
        subprocess.run(cmd, cwd=cwd, env=EXTRA_ENV_VARS)
    elif args.action == 'build_extras':
        cmd = [CMAKE_EXE, '--build', INTERMEDIATE_DIR] + EXTRA_CMAKE_ARGS
        cwd = os.path.join(THIRD_PARTY_PATH, 'pico-extras')
        subprocess.run(cmd, cwd=cwd)
    elif args.action == 'upload_circuit_python':
        firmware_filename = 'circuit_python.elf'
        cwd = os.path.join(THIRD_PARTY_PATH, 'circuit_python')
        cmd = [OPENOCD_EXE,
               '-s', OPENOCD_SCRIPTS_PATH.replace('\\', '/'),
               '-f', 'interface/cmsis-dap.cfg',
               '-f', 'target/rp2040.cfg',
               '-c', '"adapter speed 5000"',
               '-c', f'"program {firmware_filename} verify reset exit"',
               ]
        subprocess.run(cmd, cwd=cwd)
