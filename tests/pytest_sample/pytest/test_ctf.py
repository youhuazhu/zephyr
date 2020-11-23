#!/usr/bin/env python3
#
# Copyright (c) 2020 Intel Corporation.
#
# SPDX-License-Identifier: Apache-2.0
"""

"""
import os
import pytest

''' handle cmdopt '''
@pytest.fixture(autouse=True)
def pytest_cmdopt_handle(cmdopt):
    print(cmdopt)
    data_path = cmdopt
    os.environ['data_path'] = str(data_path)

''' test case '''
def test_case(cmdopt):
    if not os.path.exists(cmdopt):
        assert 0
    print(cmdopt)

if __name__ == "__main__":
    pytest.main()
