import argparse
import sys

import build_tool.build_commands as cmd

parser = argparse.ArgumentParser(prog='build_tool.bat')
parser.set_defaults(cmd_func=cmd.noop)
subparsers = parser.add_subparsers(help='Command help')

cmd_clean = subparsers.add_parser(
    'clean', help='Clean build directory')
cmd_clean.set_defaults(cmd_func=cmd.clean)

cmd_configure = subparsers.add_parser(
    'configure', help='Generate project with CMake')
cmd_configure.set_defaults(cmd_func=cmd.configure)

cmd_convert_pd = subparsers.add_parser(
    'convert_pd', help='Convert Pure Data patch to C code')
cmd_convert_pd.add_argument('program', type=str, help='Program name')
cmd_convert_pd.set_defaults(cmd_func=cmd.convert_pd)

cmd_build = subparsers.add_parser(
    'build', help='Build a program, or all programs')
cmd_build.add_argument('program', type=str, help='Program name, or "all" to compile all programs')
cmd_build.set_defaults(cmd_func=cmd.build)

cmd_deploy_swd = subparsers.add_parser(
    'deploy_swd', help='Upload firmware to rp2040 via SWD with OpenOCD')
cmd_deploy_swd.add_argument('program', type=str, help='Program to upload')
cmd_deploy_swd.set_defaults(cmd_func=cmd.deploy_swd)

cmd_pico_sdk = subparsers.add_parser(
    'pico_sdk', help='Pico SDK Helper command ')
allowed_pico_sdk_commands = [
    'configure_examples', 'build_examples',
    'configure_extras', 'build_extras',
    'upload_circuit_python'
]
cmd_pico_sdk.add_argument(
    'action', help='Action to perform within Pico SDK',
    type=str, choices=allowed_pico_sdk_commands)
cmd_pico_sdk.set_defaults(cmd_func=cmd.pico_sdk)

if len(sys.argv[1:]) > 0:
    args = parser.parse_args(sys.argv[1:])
    args.cmd_func(args)
else:
    parser.print_help()
