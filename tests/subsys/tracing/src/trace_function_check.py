#!/usr/bin/env python3
#
# Copyright (c) 2020 Intel Corporation.
#
# SPDX-License-Identifier: Apache-2.0
"""
Script to analyse tracing data with multiple backends.
"""

import argparse
import sys
import os
import time
import _thread

def parse_args():
    global args
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-b", "--backend", default='posix_port',
                        required=False, help=
                        "supported backend:posix_port, uart_port, posix_port_ctf, disabled_tracing")
    args = parser.parse_args()

# check if tracing calls produce code when the tracing system is disabled.
def tracing_disabled_handle():
    os.system('objdump -t sanity-out/native_posix/tests/subsys/tracing/tracing.transport/zephyr/zephyr.elf \
        > sanity-out/data/objdump.txt')
    # Check if the tracing functionality interfere with normal operations
    # of the operating system.
    txt = open(r"sanity-out/data/objdump.txt", "r").read()
    if txt.count("thread_switched") == 0 and txt.count("isr_enter") == 0:
        print("Not produce any code")

# Check the result for ctf with posix port.
# Dumping the trace data in text format using babeltrace.
def posix_port_ctf_handle():
    """ for posix backend handle. """
    if os.path.exists("subsys/tracing/ctf/tsdl/metadata"):
        os.system('cp subsys/tracing/ctf/tsdl/metadata sanity-out/data/')
    if os.path.exists(os.getcwd() + "/channel0_0"):
        os.system('mv ${PWD}/channel0_0 sanity-out/data/')
    else:
        os.system("cp sanity-out/native_posix/tests/subsys/tracing/" + \
            "tracing.transport.posix.ctf/channel0_0 sanity-out/data/")

    os.system('babeltrace sanity-out/data/ > sanity-out/data/tracing.txt')
    check_results()

# Check the result with posix port.
# Dumping the trace data in text format using babeltrace.
def posix_port_handle():
    """ for posix backend handle. """
    if os.path.exists(os.getcwd() + "/channel0_0"):
        os.system('mv ${PWD}/channel0_0 sanity-out/data/tracing.txt')
    else:
        os.system("cp sanity-out/native_posix/tests/subsys/tracing/" + \
            "tracing.transport.native_posix/channel0_0 sanity-out/data/tracing.txt")
    check_results()

# Check the result with posix port.
# Dumping the trace data in text format using babeltrace.
def uart_port_handle():
    """ for uart backend handle. """
    if os.path.exists(os.getcwd() + "/channel0_0"):
        os.system('mv ${PWD}/channel0_0 sanity-out/data/tracing.txt')
    else:
        os.system("cp sanity-out/qemu_x86/tests/subsys/tracing/" + \
            "tracing.transport.uart/channel0_0 sanity-out/data/tracing.txt")
    check_results()

def check_results():
    """ Check the current phase of operation (thread, ISR, idle thread, etc.)
    based on the trace data. """
    txt = open(r"sanity-out/data/tracing.txt", "r").read()

    # check if thread object be traced.
    if txt.count("thread_switched") > 0:
        print("thread_switched")

    # check if ISR object be traced.
    if txt.count("isr_enter") > 0:
        print("isr_enter")

    # check if idle thread object be traced.
    if txt.count("idle") > 0:
        print("idle thread")

def main():
    """ Main Entry Point """
    parse_args()

    output_file = args.backend.strip()
    ZEPHYR_BASE = os.path.abspath(os.path.join(os.path.dirname(__file__), "../../../.."))

    # change to work directory and create data directory.
    os.chdir(ZEPHYR_BASE)
    if os.path.exists("sanity-out/data"):
        os.system('rm -rf sanity-out/data')
    os.makedirs("sanity-out/data")

    # The tracing system supported multiple backends.
    if output_file == "posix_port_ctf":
        posix_port_ctf_handle()
    elif output_file == "posix_port":
        posix_port_handle()
    elif output_file == "uart_port":
        uart_port_handle()
    elif output_file == "disabled_tracing":
        tracing_disabled_handle()
    else:
        print(output_file + " not supported.")
        sys.exit(1)

if __name__ == "__main__":
    main()
