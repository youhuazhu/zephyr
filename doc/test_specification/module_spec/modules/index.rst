.. _module_TestCases:

module Test cases
##################

Consolidated module Test Specification According to IEC 61508:2010,
we provide details of the test cases, test data, and test objects to be used for verification,

A test case consists of a set of input values, execution precondition, expected results and
executed post condition, and developed to cover a certain test condition. A test scenario
is a test procedure that has one or many relations with test cases, meaning a scenario
can have multiple test cases.

We use Zephyr Test Framework (`Ztest <https://docs.zephyrproject.org/latest/guides/test/ztest.html>`_) to handle test cases, it provides a simple testing framework intended to be used during test cases development.
It provides basic assertion macros and a generic test structure.

normally, we have 2 kinds of cases for module test:

- normal testcases
- negative testcases

**Test result:**

 All test cases shouldn't be failed by `zassert` when test cases running, and all result(pass/fail) will be collected in test report.

.. toctree::
   :maxdepth: 2
   :glob:

   smp
