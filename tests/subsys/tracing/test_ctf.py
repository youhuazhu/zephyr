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
import pytest

# Check the result for ctf with posix port.
# Dumping the trace data in text format using babeltrace.
@pytest.fixture(autouse=True)
def uart_ctf_handle():
    """ handle ctf data from uart backend """
    #capture trace date for 5s.
    #time.sleep(5)
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

def test_ctf():
    """ Main Entry Point """
    # check if thread/isr/semaphore object be traced.
    txt = open("ctf/tracing.txt", "r").read()
    assert txt.count("thread_switched") > 0 
    assert txt.count("semaphore_give") > 0

if __name__ == "__main__":
     pytest.main(["-s", "tests/subsys/tracing/test_ctf.py"])
