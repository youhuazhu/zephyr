/*
 * Copyright (c) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @defgroup subsys_tracing_tests subsys tracing
 * @ingroup all_tests
 * @{
 * @}
 */

#include <ztest.h>
#ifdef CONFIG_ARCH_POSIX
#include <unistd.h>
#include <stdlib.h>
#endif

/* size of stack area used by each thread */
#define STACK_SIZE (512 + CONFIG_TEST_EXTRA_STACKSIZE)
#define LOOP_TIMES 200

#define dummy_test(_name) \
	static void _name(void) \
	{ \
		ztest_test_skip(); \
	}

static struct k_thread thread;
K_THREAD_STACK_DEFINE(thread_stack, STACK_SIZE);

/* define semaphores */
K_SEM_DEFINE(thread1_sem, 1, 1);
K_SEM_DEFINE(thread2_sem, 0, 1);

/* thread handle for switch */
void thread_handle(void *p1, void *self_sem, void *other_sem)
{
	for (int i = 0; i < LOOP_TIMES; i++) {
		/* take my semaphore */
		k_sem_take(self_sem, K_FOREVER);

		/* wait a while, then let other thread have a turn */
		k_sleep(K_MSEC(500));

		k_sem_give(other_sem);
	}
}

/* spawn thread to generate more tracing data */
void threads_loop(void)
{
	k_thread_create(&thread, thread_stack, STACK_SIZE,
		thread_handle, NULL, &thread2_sem, &thread1_sem,
		K_PRIO_PREEMPT(0), K_INHERIT_PERMS,
		K_NO_WAIT);

	thread_handle(NULL, &thread1_sem, &thread2_sem);
}

/**
 * we need to use external tools to handle tracing data
 * which produced by qemu/posix platform with different
 * backends. and use popen api to execute the tools, so
 * below test cases only avalible on posix arch.
 */
#ifdef CONFIG_ARCH_POSIX
#define MAX_BYTES 1024
#define TRACE_TYPES 3

char *trace_result[TRACE_TYPES] = {
	"thread_switched",
	"isr_enter",
	"idle thread" };

char *trace_disabled[] = {
	"Not produce any code" };

void tracing_operation_posix(char type[], char *expect_result[], int num)
{
	char result_buf[MAX_BYTES], command[MAX_BYTES];
	int index = 0, rc = 0;
	FILE *fp;

	snprintf(command, MAX_BYTES,
		"python3 "
		"$ZEPHYR_BASE/tests/subsys/tracing/src/trace_function_check.py -b ");
	strcat(command, type);

	/* execute the python command to analyse tracing data. */
	fp = popen(command, "r");
	if (fp == NULL) {
		perror("fail to popen！");
		zassert_false(1, "fail to run external command.");
		return;
	}

	/* chech the result from python script. */
	while (fgets(result_buf, sizeof(result_buf), fp) != NULL) {
		/* remove the redundant blank spaces. */
		if ('\n' == result_buf[strlen(result_buf)-1]) {
			result_buf[strlen(result_buf)-1] = '\0';
		}

		if (index < num) {
			zassert_true(!strcmp(result_buf, expect_result[index]),
				"bad value read.");
			index++;
		}
	}

	/* index equal 0 mean doesn't zassert the return value as above code. */
	if (index == 0)
		zassert_false(1, "doesn't zassert anything.");

	rc = pclose(fp);

	if (rc == -1) {
		perror("fail to close pclose.");
	}
}

/**
 * @brief Test if product redundant code when tracing disabled.
 *
 * @details
 * - All tracing calls shall not be producing code when
 * the tracing system is disable.
 *
 * @ingroup subsys_tracing_tests
 */
void test_tracing_operation_disabled(void)
{
#if defined(CONFIG_TRACING)
	ztest_test_skip();
#endif
	tracing_operation_posix("disabled_tracing", trace_disabled, 1);
}

/**
 * @brief Test if support CTF and dumping the trace data.
 *
 * @details
 * - The tracing system if support the common tracing format (CTF).
 * - The tracing system shall support dumping the trace data in text format
 * using babeltrace in the system.
 *
 * @ingroup subsys_tracing_tests
 */
void test_tracing_operation_posix_ctf(void)
{
#ifndef CONFIG_TRACING_CTF
	ztest_test_skip();
#endif
	tracing_operation_posix("posix_port_ctf", trace_result, TRACE_TYPES);
}

/**
 * @brief Test if support tracing thread, ISR, idle thread operation.
 *
 * @details
 * - The consumer of the tracing data shall be able to identify and show
 * the current phase of operation (thread, ISR, idle thread, etc.)
 * based on the provided trace data.
 *
 * @ingroup subsys_tracing_tests
 */
void test_tracing_operation_posix(void)
{
#if !defined(CONFIG_TRACING) || defined(CONFIG_TRACING_CTF)
	ztest_test_skip();
#endif
	tracing_operation_posix("posix_port", trace_result, TRACE_TYPES);
}

/**
 * @brief Test if support uart backend.
 *
 * @details
 * - Using qemu platform to imitate uart backend.
 * - and dumping the trace data into a file with text format.
 *
 * @ingroup subsys_tracing_tests
 */
void test_tracing_operation_uart(void)
{
#if !defined(CONFIG_TRACING)
	ztest_test_skip();
#endif
	tracing_operation_posix("uart_port", trace_result, TRACE_TYPES);
}
#else
/* Skip this case for non-posix platform. */
dummy_test(test_tracing_operation_posix);
dummy_test(test_tracing_operation_posix_ctf);
dummy_test(test_tracing_operation_uart);
dummy_test(test_tracing_operation_disabled);
#endif

void test_main(void)
{
	k_thread_access_grant(k_current_get(),
				  &thread, &thread_stack,
				  &thread1_sem, &thread2_sem);

	/* generate tracing data. */
	threads_loop();
	k_sleep(K_MSEC(500));

	ztest_test_suite(test_tracing,
			 ztest_unit_test(test_tracing_operation_posix),
			 ztest_unit_test(test_tracing_operation_posix_ctf),
			 ztest_unit_test(test_tracing_operation_uart),
			 ztest_unit_test(test_tracing_operation_disabled)
			 );

	ztest_run_test_suite(test_tracing);
}
