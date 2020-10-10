#!/usr/bin/env python3
#
# Copyright (c) 2020 Intel Corporation.
#
# SPDX-License-Identifier: Apache-2.0
"""
Script to handle trace data with uart backend.
"""
import os
import time
import subprocess
from shutil import copyfile
import pytest

# Dumping the trace data in text format using babeltrace.
@pytest.fixture(autouse=True)
def uart_ctf_handle(cmdopt):
    """ handle ctf data from uart backend """
    #capture trace date for 5s.
    time.sleep(5)
    data_path = cmdopt
    os.environ['data_path'] = str(data_path)

    if not os.path.exists(data_path + "/ctf"):
        os.mkdir(data_path + "/ctf")

    if os.path.exists("subsys/tracing/ctf/tsdl/metadata"):
        copyfile("subsys/tracing/ctf/tsdl/metadata", data_path + "/ctf/metadata")

    os.chdir(data_path)
    copyfile("channel0_0", "ctf/channel0_0")

    if subprocess.call("babeltrace ctf/ > ctf/tracing.txt 2>&1", shell=True) == 127:
        print("skip: please install babeltrace with：sudo apt install babeltrace.")

# check if thread/semaphore object be traced.
def test_ctf():
    """ Main Entry Point """
    txt = open("ctf/tracing.txt", "r").read()
    assert txt.count("thread_switched") > 0
    assert txt.count("semaphore_give") > 0

if __name__ == "__main__":
    pytest.main()
