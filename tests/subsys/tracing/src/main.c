/*
 * Copyright (c) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <ztest.h>
#if defined(CONFIG_TRACING_BACKEND_UART) || defined(CONFIG_TRACING_BACKEND_USB)
#include "../../../../subsys/tracing/include/tracing_backend.h"
#endif

/**
 * @defgroup subsys_tracing_tests subsys tracing
 * @ingroup all_tests
 * @{
 * @}
 */

/* size of stack area used by each thread */
#define STACK_SIZE (512 + CONFIG_TEST_EXTRA_STACKSIZE)
#define LOOP_TIMES 10

bool thread_switched_found;
bool sys_trace_void_found;
bool sys_trace_semaphore_found;

static struct k_thread thread;
K_THREAD_STACK_DEFINE(thread_stack, STACK_SIZE);

/* define 2 semaphores */
K_SEM_DEFINE(thread1_sem, 1, 1);
K_SEM_DEFINE(thread2_sem, 0, 1);

/* thread handle for switch */
void thread_handle(void *p1, void *self_sem, void *other_sem)
{
#ifdef CONFIG_TRACING_CTF
	/* wait harness to handle the tracing data, this case will timeout
	 * if harness handle fail.
	 */
	while (1) {
#else
	for (int i = 0; i < LOOP_TIMES; i++) {
#endif
		/* take my semaphore */
		k_sem_take(self_sem, K_FOREVER);
		/* wait for a while, then let other thread have a turn. */
		k_sleep(K_MSEC(100));

		k_sem_give(other_sem);
	}
}

#if (defined(CONFIG_TRACING_BACKEND_UART) || defined(CONFIG_TRACING_BACKEND_USB)) && !defined(CONFIG_TRACING_CTF)
static void tracing_backends_output(
		const struct tracing_backend *backend,
		uint8_t *data, uint32_t length)
{
	uint8_t recv_char[100], recv_index = 0;

	for (uint32_t i = 0; i < length; i++) {
		recv_char[recv_index] = data[i];
		recv_index++;

		if ((data[i] == '\n') || (data[i] == '\r')) {
			if (strstr(recv_char, "sys_trace_thread_switched") != NULL) {
				thread_switched_found = true;
			}
			if (strstr(recv_char, "sys_trace_void") != NULL) {
				sys_trace_void_found = true;
			}
			if (strstr(recv_char, "sys_trace_semaphore") != NULL) {
				sys_trace_semaphore_found = true;
			}

			recv_index = 0;
			memset(recv_char, 0, sizeof(recv_char));
		}
	}
}
#endif

#if defined(CONFIG_TRACING_BACKEND_UART) && !defined(CONFIG_TRACING_CTF)
const struct tracing_backend_api tracing_uart_backend_api = {
	.init = NULL,
	.output  = tracing_backends_output
};

TRACING_BACKEND_DEFINE(tracing_backend_uart, tracing_uart_backend_api);
#endif

#ifdef CONFIG_TRACING_BACKEND_USB
const struct tracing_backend_api tracing_usb_backend_api = {
	.output = tracing_backends_output
};

TRACING_BACKEND_DEFINE(tracing_backend_usb, tracing_usb_backend_api);
#endif

/*
 * spawn a thread to generate more tracing data about
 * thread switch and semaphore.
 */
void generate_more_tracing_data(void)
{
	k_sleep(K_MSEC(1000));
	k_thread_create(&thread, thread_stack, STACK_SIZE,
		thread_handle, NULL, &thread2_sem, &thread1_sem,
		K_PRIO_PREEMPT(0), K_INHERIT_PERMS,
		K_NO_WAIT);

	thread_handle(NULL, &thread1_sem, &thread2_sem);
}
/**
 * @brief Test tracing data which produced from backends.
 *
 * @details
 * - Check the tracing data in backends, it should include thread/semaphore.. info,
 * if not, the related variable should be false.
 *
 * @ingroup subsys_tracing_tests
 */
void test_tracing_function(void)
{
#ifdef CONFIG_TRACING_CTF
	ztest_test_skip();
#endif
	generate_more_tracing_data();
#ifdef CONFIG_TRACING
	zassert_true(thread_switched_found, "doesn't find thread_switched.");
	zassert_true(sys_trace_void_found, "doesn't find sys_trace_void.");
	zassert_true(sys_trace_semaphore_found,
			"doesn't find sys_trace_semaphore.");
#else
	/* Should produce any tracing data when tracing disabled. */
	zassert_false(thread_switched_found, "shouldn't have trace data.");
	zassert_false(sys_trace_void_found, "shouldn't have trace data.");
	zassert_false(sys_trace_semaphore_found, "shouldn't have trace data.");
#endif
}

void test_tracing_function_CTF(void)
{
#ifndef CONFIG_TRACING_CTF
	ztest_test_skip();
#endif
	generate_more_tracing_data();
}

void test_main(void)
{
	k_thread_access_grant(k_current_get(),
				  &thread, &thread_stack,
				  &thread1_sem, &thread2_sem);

	ztest_test_suite(test_tracing,
			 ztest_unit_test(test_tracing_function),
			 ztest_unit_test(test_tracing_function_CTF)
			 );

	ztest_run_test_suite(test_tracing);
}
