#!/usr/bin/env python3
#
# Copyright (c) 2020 Intel Corporation.
#
# SPDX-License-Identifier: Apache-2.0
"""
Script to handle trace data with uart backend.
"""
import os
import sys
import time
import subprocess

# Check the result for ctf with posix port.
# Dumping the trace data in text format using babeltrace.
def uart_ctf_handle():
    """ handle ctf data from uart backend """
    #capture trace date for 5s.
    time.sleep(5)
    data_path = sys.argv[1]
    os.environ['data_path'] = str(data_path)

    if not os.path.exists(data_path + "/ctf"):
        os.mkdir(data_path + "/ctf")

    if os.path.exists("subsys/tracing/ctf/tsdl/metadata"):
        os.system('cp subsys/tracing/ctf/tsdl/metadata ${data_path}/ctf/')

    os.chdir(data_path)
    os.system('cp channel0_0 ctf/')

    if subprocess.call("babeltrace ctf/ > ctf/tracing.txt 2>&1", shell=True) == 127:
        print("skip: please install babeltrace with：sudo apt install babeltrace.")
    else:
        # check if thread/isr/semaphore object be traced.
        txt = open("ctf/tracing.txt", "r").read()
        if txt.count("thread_switched") > 0 and txt.count("semaphore_give") > 0:
            print("pass: thread/semaphore/isr trace data all found.")
        else:
            print("fail: thread/semaphore/isr trace data not all found.")

def main():
    """ Main Entry Point """
    uart_ctf_handle()

if __name__ == "__main__":
    main()
