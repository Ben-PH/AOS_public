#!/usr/bin/python3
#
# Copyright 2019, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the GNU General Public License version 2. Note that NO WARRANTY is provided.
# See "LICENSE_GPLv2.txt" for details.
#
# @TAG(DATA61_GPL)
#
import argparse
import bisect
import pdb
import re
import subprocess


def parse_args():
    parser = argparse.ArgumentParser(
        description='Parse a list of addresses generated by print_backtrace() into something approximating a stacktrace by extracting information from nm and objdump.')
    parser.add_argument('-l', '--line', action='store_true', default=False,
                        help='pass -l (show lines) to objdump.')
    parser.add_argument('-S', '--source', action='store_true', default=False,
                        help='pass -S (show sources) to objdump.')
    parser.add_argument('-w', '--width', type=int, default=2,
                        help='Lines from the objdump to show either side of the target address.', choices=range(0, 8))
    parser.add_argument('--start-address', type=hex, help='pass --start-address to objdump.')
    parser.add_argument('--stop-address', type=hex, help='pass --stop-address to objdump.')
    parser.add_argument('-t', '--toolchain', default='aarch64-linux-gnu-',
                        help='Prefix for the toolchain to use.')
    parser.add_argument('file', type=argparse.FileType(
        'r'), help="Path to the sos binary (eg. build/projects/aos/sos/sos)")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    objdump_cmd = [args.toolchain + 'objdump', '-d']
    if args.start_address:
        objdump_cmd += ['--start-address', args.start_address]
    if args.stop_address:
        objdump_cmd += ['--stop-address', args.stop_address]
    if args.line:
        objdump_cmd += ['-l']
    if args.source:
        objdump_cmd += ['-S']
    objdump_cmd += [args.file.name]

    print("Enter whitespace separated addresses, <Ctrl>+D to process and exit\n> ")
    addresses = []
    while True:
        try:
            addr = input("> ")
            for i in addr.split():
                try:
                    val = int(i, 16)
                    addresses.append(val)
                except ValueError:
                    print('ERROR: \'' + i + "\' is not a valid hex address")
                    exit(1)
        except EOFError:
            break

    print("Processing.", end='', flush='True')

    # run nm and parse the symbol table
    nm = subprocess.run([args.toolchain + 'nm', '-n', args.file.name],
                        stdout=subprocess.PIPE, universal_newlines=True)
    print(".", end='', flush='True')
    if nm.returncode != 0:
        print(nm.stderr)
        exit(1)
    nm_dict = {}  # a dict of addr = symbol
    ordered_addrs = []  # ordered list of addresses
    nm_regex = re.compile('([0-9A-Fa-f]{8,16})\s[A-Za-z]\s(.*)')
    for line in nm.stdout.split('\n'):
        nm_match = nm_regex.match(line)
        if nm_match:
            addr = int(nm_match.group(1), 16)
            nm_dict[addr] = nm_match.group(2)
            ordered_addrs.append(addr)

    # load up the objdump
    dis = subprocess.run(objdump_cmd, stdout=subprocess.PIPE, universal_newlines=True)
    print(".", end='', flush='True')
    if dis.returncode != 0:
        print(dis.stderr)
        exit(1)
    dis_output = dis.stdout.split('\n')
    print(".")

    level = 0
    for addr in addresses:
        print ('--------------------------------------------------------------------------------')
        # find the name of the symbol
        if addr in nm_dict:
            name = nm_dict[addr]
        else:
            index = bisect.bisect_right(ordered_addrs, addr)
            if index:
                name = nm_dict.get(ordered_addrs[index-1])

        if name:
            print ("{0} {1:16x} in {2}".format(level, addr, name))
        else:
            print ("{0} {1:16x} can not be resolved".format(level, addr))
            continue
        level += 1

        # now output some of the dissassembly line
        start_addr = addr - args.width * 4
        end_addr = addr + (args.width + 1) * 4

        dis_regex = re.compile('\s*([0-9A-Fa-f]{4,16}):')
        output = False
        bold = False
        for line in dis_output:
            dis_match = dis_regex.match(line)
            if dis_match:
                line_addr = int(dis_match.group(1), 16)
                if line_addr == addr:
                    bold = True
                if line_addr >= start_addr:
                    output = True
                if line_addr == end_addr:
                    break
            if bold:
                print('\033[1m', end='')
            if output:
                print(line)
            if bold:
                print("\033[0m", end='', flush=True)
                bold = False
