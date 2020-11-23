# SPDX-License-Identifier: Apache-2.0
import re
import os
import subprocess
from collections import OrderedDict
import xml.etree.ElementTree as ET

result_re = re.compile(".*(PASS|FAIL|SKIP) - (test_)?(.*)")

class Harness:
    GCOV_START = "GCOV_COVERAGE_DUMP_START"
    GCOV_END = "GCOV_COVERAGE_DUMP_END"
    FAULT = "ZEPHYR FATAL ERROR"
    RUN_PASSED = "PROJECT EXECUTION SUCCESSFUL"
    RUN_FAILED = "PROJECT EXECUTION FAILED"

    def __init__(self):
        self.state = None
        self.type = None
        self.regex = []
        self.matches = OrderedDict()
        self.ordered = True
        self.repeat = 1
        self.tests = {}
        self.id = None
        self.fail_on_fault = True
        self.fault = False
        self.capture_coverage = False
        self.next_pattern = 0
        self.record = None
        self.recording = []
        self.fieldnames = []
        self.ztest = False
        self.running_dir = None
        self.source_dir = None
        self.pytest_root = 'pytest'
        self.pytest_timeout = 15

    def configure(self, instance):
        config = instance.testcase.harness_config
        self.id = instance.testcase.id
        if "ignore_faults" in instance.testcase.tags:
            self.fail_on_fault = False

        if config:
            self.type = config.get('type', None)
            self.regex = config.get('regex', [])
            self.repeat = config.get('repeat', 1)
            self.ordered = config.get('ordered', True)
            self.record = config.get('record', {})
            self.running_dir = config.get('running_dir', None)
            self.source_dir = config.get('source_dir', None)
            self.pytest_root = config.get('pytest_root', 'pytest')
            self.pytest_timeout = config.get('pytest_timeout', 'pytest')

    def process_test(self, line):

        if self.RUN_PASSED in line:
            if self.fault:
                self.state = "failed"
            else:
                self.state = "passed"

        if self.RUN_FAILED in line:
            self.state = "failed"

        if self.fail_on_fault:
            if self.FAULT == line:
                self.fault = True

        if self.GCOV_START in line:
            self.capture_coverage = True
        elif self.GCOV_END in line:
            self.capture_coverage = False

class Console(Harness):

    def configure(self, instance):
        super(Console, self).configure(instance)
        if self.type == "one_line":
            self.pattern = re.compile(self.regex[0])
        elif self.type == "multi_line":
            self.patterns = []
            for r in self.regex:
                self.patterns.append(re.compile(r))

    def handle(self, line):
        if self.type == "one_line":
            if self.pattern.search(line):
                self.state = "passed"
        elif self.type == "multi_line" and self.ordered:
            if (self.next_pattern < len(self.patterns) and
                self.patterns[self.next_pattern].search(line)):
                self.next_pattern += 1
                if self.next_pattern >= len(self.patterns):
                    self.state = "passed"
        elif self.type == "multi_line" and not self.ordered:
            for i, pattern in enumerate(self.patterns):
                r = self.regex[i]
                if pattern.search(line) and not r in self.matches:
                    self.matches[r] = line
            if len(self.matches) == len(self.regex):
                self.state = "passed"

        if self.fail_on_fault:
            if self.FAULT in line:
                self.fault = True

        if self.GCOV_START in line:
            self.capture_coverage = True
        elif self.GCOV_END in line:
            self.capture_coverage = False


        if self.record:
            pattern = re.compile(self.record.get("regex", ""))
            match = pattern.search(line)
            if match:
                csv = []
                if not self.fieldnames:
                    for k,v in match.groupdict().items():
                        self.fieldnames.append(k)

                for k,v in match.groupdict().items():
                    csv.append(v.strip())
                self.recording.append(csv)

        if self.state == "passed":
            self.tests[self.id] = "PASS"
        else:
            self.tests[self.id] = "FAIL"

        self.process_test(line)

class Pytest(Harness):
    ''' To keep artifacts of pytest in running_dir which is hard to be
        determined before sanitychek, pass this directory by "--cmdopt".
        On pytest end, add a command line option and provide the cmdopt
        through a fixture function '''

    def handle(self, line):
        cmd = [
			'pytest',
			'-s',
			os.path.join(self.source_dir, self.pytest_root),
			'--cmdopt',
			self.running_dir,
			'--junit-xml',
			os.path.join(self.running_dir, 'report.html')
        ]

        with subprocess.Popen(cmd) as proc:
            try:
                proc.wait(self.pytest_timeout)
                tree = ET.parse(os.path.join(self.running_dir, "report.html"))
                root = tree.getroot()
                for child in root:
                    if child.tag == 'testsuite':
                        if child.attrib['failures'] != '0':
                            self.state = "failed"
                        elif child.attrib['skipped'] != '0':
                            self.state = "skipped"
                        elif child.attrib['errors'] != '0':
                            self.state = "errors"
                        else:
                            self.state = "passed"
            except subprocess.TimeoutExpired:
                proc.kill()
                self.state = "failed"
            except ET.ParseError:
                self.state = "failed"
            except IOError:
                self.state = "failed"

        if self.state == "passed":
            self.tests[self.id] = "PASS"
        elif self.state == "skipped":
            self.tests[self.id] = "SKIP"
        else:
            self.tests[self.id] = "FAIL"

        self.process_test(line)

class Test(Harness):
    RUN_PASSED = "PROJECT EXECUTION SUCCESSFUL"
    RUN_FAILED = "PROJECT EXECUTION FAILED"

    def handle(self, line):
        match = result_re.match(line)
        if match and match.group(2):
            name = "{}.{}".format(self.id, match.group(3))
            self.tests[name] = match.group(1)
            self.ztest = True

        if self.RUN_PASSED in line:
            if self.fault:
                self.state = "failed"
            else:
                self.state = "passed"

        if self.RUN_FAILED in line:
            self.state = "failed"

        if self.fail_on_fault:
            if self.FAULT in line:
                self.fault = True

        if not self.ztest and self.state:
            if self.state == "passed":
                self.tests[self.id] = "PASS"
            else:
                self.tests[self.id] = "FAIL"

        if self.GCOV_START in line:
            self.capture_coverage = True
        elif self.GCOV_END in line:
            self.capture_coverage = False

        self.process_test(line)

class Ztest(Test):
    pass
