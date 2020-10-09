.. _modue_spec_environment:

Environment
############

Description of module testing environment, including software, hardware environment.

For Software module test, the following docs and code shall be available:

 - Zephyr kernel code
 - Software module Test Plan

**1. In Scope**:

This document is intended to address the following functional safety requirements.

- IEC 61508-2:2010 (systems/hardware development)

    7.9.2.3, 7.9.2.4

- IEC 61508-3:2010 (software development)

    7.9.2.6, 7.9.2.7

**2. Out of Scope**:

Information specific to the verification plan and verification report are not addressed in this document.

**3. Output**:

The following record and code are updated and created during module & HSI Test Phase testing.

 - Software module Test Cases
 - Software module Test Reports

**4. Test Platforms**

Two reference boards are selected and used for hardware/software integration test. One board is chosen for Intel x86 CPU architecture and another board is chosen for ARM CPU architecture. The two boards are:

+-----------------+--------------+------------+----------------------------------+
| Board name      | CPU family   | Vendor name|        Link                      |
+=================+==============+============+==================================+
| MIMXRT1050-EVKB | Cortex®-M7   |  NXP       |https://docs.zephyrproject.org/   |
| Arm®            |              |            |latest/boards/arm/mimxrt1050_evk/ |
|                 |              |            |doc/index.html#board-revisions    |
+-----------------+--------------+------------+----------------------------------+
| Apollo Lake UP2 |Intel® Pentium|    Intel   |https://docs.zephyrproject.org/   |
|                 |N4200         |            |latest/boards/x86/up_squared/     |
|                 |              |            |doc/index.html                    |
+-----------------+--------------+------------+----------------------------------+

**5. Setup environment**:

We use a script named sanitycheck to run module testcases.
This script scans for the set of unit test applications in the git repository
and attempts to execute them. By default, it tries to build each test
case on boards marked as default in the board definition file.

The default options will build the majority of the tests on a defined set of
boards and will run in an emulated environment if available for the
architecture or configuration being tested.

In normal use, sanitycheck runs a limited set of kernel tests (inside
an emulator).  Because of its limited test execution coverage, sanitycheck
cannot guarantee local changes will succeed in the full build
environment, but it does sufficient testing by building samples and
tests for different boards and different configurations to help keep the
complete code tree buildable.

When using (at least) one ``-v`` option, sanitycheck's console output
shows for every test how the test is run (qemu, native_posix, etc.) or
whether the binary was just built.

To run the script in the local tree, follow the steps below:

::

        $ source zephyr-env.sh
        $ ./scripts/sanitycheck

If you have a system with a large number of cores, you can build and run
all possible tests using the following options:

::

        $ ./scripts/sanitycheck --all --enable-slow

This will build for all available boards and run all applicable tests in
a simulated (for example QEMU) environment.

The list of command line options supported by sanitycheck can be viewed using::

        $ ./scripts/sanitycheck --help
