/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * The purpose of this file is to provide essential/minimal kernel structure
 * definitions, so that they can be used without including kernel.h.
 *
 * The following rules must be observed:
 *  1. kernel_structs.h shall not depend on kernel.h both directly and
 *    indirectly (i.e. it shall not include any header files that include
 *    kernel.h in their dependency chain).
 *  2. kernel.h shall imply kernel_structs.h, such that it shall not be
 *    necessary to include kernel_structs.h explicitly when kernel.h is
 *    included.
 */

#ifndef ZEPHYR_KERNEL_INCLUDE_KERNEL_STRUCTS_H_
#define ZEPHYR_KERNEL_INCLUDE_KERNEL_STRUCTS_H_

#if !defined(_ASMLANGUAGE)
#include <sys/atomic.h>
#include <zephyr/types.h>
#include <sched_priq.h>
#include <sys/dlist.h>
#include <sys/util.h>
#include <sys/sys_heap.h>
#endif

#define K_NUM_PRIORITIES \
	(CONFIG_NUM_COOP_PRIORITIES + CONFIG_NUM_PREEMPT_PRIORITIES + 1)

#define K_NUM_PRIO_BITMAPS ((K_NUM_PRIORITIES + 31) >> 5)

/*
 * Bitmask definitions for the struct k_thread.thread_state field.
 *
 * Must be before kerneL_arch_data.h because it might need them to be already
 * defined.
 */

/* states: common uses low bits, arch-specific use high bits */

/* Not a real thread */
#define _THREAD_DUMMY (BIT(0))

/* Thread is waiting on an object */
#define _THREAD_PENDING (BIT(1))

/* Thread has not yet started */
#define _THREAD_PRESTART (BIT(2))

/* Thread has terminated */
#define _THREAD_DEAD (BIT(3))

/* Thread is suspended */
#define _THREAD_SUSPENDED (BIT(4))

/* Thread is being aborted (SMP only) */
#define _THREAD_ABORTING (BIT(5))

/* Thread was aborted in interrupt context (SMP only) */
#define _THREAD_ABORTED_IN_ISR (BIT(6))

/* Thread is present in the ready queue */
#define _THREAD_QUEUED (BIT(7))

/* end - states */

#ifdef CONFIG_STACK_SENTINEL
/* Magic value in lowest bytes of the stack */
#define STACK_SENTINEL 0xF0F0F0F0
#endif

/* lowest value of _thread_base.preempt at which a thread is non-preemptible */
#define _NON_PREEMPT_THRESHOLD 0x0080

/* highest value of _thread_base.preempt at which a thread is preemptible */
#define _PREEMPT_THRESHOLD (_NON_PREEMPT_THRESHOLD - 1)

#if !defined(_ASMLANGUAGE)

struct _ready_q {
#ifndef CONFIG_SMP
	/* always contains next thread to run: cannot be NULL */
	struct k_thread *cache;
#endif

#if defined(CONFIG_SCHED_DUMB)
	sys_dlist_t runq;
#elif defined(CONFIG_SCHED_SCALABLE)
	struct _priq_rb runq;
#elif defined(CONFIG_SCHED_MULTIQ)
	struct _priq_mq runq;
#endif
};

typedef struct _ready_q _ready_q_t;

struct _cpu {
	/* nested interrupt count */
	u32_t nested;

	/* interrupt stack pointer base */
	char *irq_stack;

	/* currently scheduled thread */
	struct k_thread *current;

	/* one assigned idle thread per CPU */
	struct k_thread *idle_thread;

#if (CONFIG_NUM_METAIRQ_PRIORITIES > 0) && (CONFIG_NUM_COOP_PRIORITIES > 0)
	/* Coop thread preempted by current metairq, or NULL */
	struct k_thread *metairq_preempted;
#endif

#ifdef CONFIG_TIMESLICING
	/* number of ticks remaining in current time slice */
	int slice_ticks;
#endif

	u8_t id;

#ifdef CONFIG_SMP
	/* True when _current is allowed to context switch */
	u8_t swap_ok;
#endif
};

typedef struct _cpu _cpu_t;

struct z_kernel {
	/* For compatibility with pre-SMP code, union the first CPU
	 * record with the legacy fields so code can continue to use
	 * the "_kernel.XXX" expressions and assembly offsets.
	 */
	union {
		struct _cpu cpus[CONFIG_MP_NUM_CPUS];
#ifndef CONFIG_SMP
		struct {
			/* nested interrupt count */
			u32_t nested;

			/* interrupt stack pointer base */
			char *irq_stack;

			/* currently scheduled thread */
			struct k_thread *current;
		};
#endif
	};

#ifdef CONFIG_SYS_CLOCK_EXISTS
	/* queue of timeouts */
	sys_dlist_t timeout_q;
#endif

#ifdef CONFIG_SYS_POWER_MANAGEMENT
	s32_t idle; /* Number of ticks for kernel idling */
#endif

	/*
	 * ready queue: can be big, keep after small fields, since some
	 * assembly (e.g. ARC) are limited in the encoding of the offset
	 */
	struct _ready_q ready_q;

#ifdef CONFIG_FP_SHARING
	/*
	 * A 'current_sse' field does not exist in addition to the 'current_fp'
	 * field since it's not possible to divide the IA-32 non-integer
	 * registers into 2 distinct blocks owned by differing threads.  In
	 * other words, given that the 'fxnsave/fxrstor' instructions
	 * save/restore both the X87 FPU and XMM registers, it's not possible
	 * for a thread to only "own" the XMM registers.
	 */

	/* thread that owns the FP regs */
	struct k_thread *current_fp;
#endif

#if defined(CONFIG_THREAD_MONITOR)
	struct k_thread *threads; /* singly linked list of ALL threads */
#endif
};

typedef struct z_kernel _kernel_t;

extern struct z_kernel _kernel;

#ifdef CONFIG_SMP

/* True if the current context can be preempted and migrated to
 * another SMP CPU.
 */
bool z_smp_cpu_mobile(void);

#define _current_cpu ({ __ASSERT_NO_MSG(!z_smp_cpu_mobile()); \
			arch_curr_cpu(); })
#define _current k_current_get()

#else
#define _current_cpu (&_kernel.cpus[0])
#define _current _kernel.current
#endif

#define _timeout_q _kernel.timeout_q

/* kernel wait queue record */

#ifdef CONFIG_WAITQ_SCALABLE

typedef struct {
	struct _priq_rb waitq;
} _wait_q_t;

extern bool z_priq_rb_lessthan(struct rbnode *a, struct rbnode *b);

#define Z_WAIT_Q_INIT(wait_q) { { { .lessthan_fn = z_priq_rb_lessthan } } }

#else

typedef struct {
	sys_dlist_t waitq;
} _wait_q_t;

#define Z_WAIT_Q_INIT(wait_q) { SYS_DLIST_STATIC_INIT(&(wait_q)->waitq) }

#endif

/* kernel timeout record */

struct _timeout;
typedef void (*_timeout_func_t)(struct _timeout *t);

struct _timeout {
	sys_dnode_t node;
	s32_t dticks;
	_timeout_func_t fn;
};

/* kernel spinlock type */

struct k_spinlock {
#ifdef CONFIG_SMP
	atomic_t locked;
#endif

#ifdef CONFIG_SPIN_VALIDATE
	/* Stores the thread that holds the lock with the locking CPU
	 * ID in the bottom two bits.
	 */
	uintptr_t thread_cpu;
#endif

#if defined(CONFIG_CPLUSPLUS) && !defined(CONFIG_SMP) && \
	!defined(CONFIG_SPIN_VALIDATE)
	/* If CONFIG_SMP and CONFIG_SPIN_VALIDATE are both not defined
	 * the k_spinlock struct will have no members. The result
	 * is that in C sizeof(k_spinlock) is 0 and in C++ it is 1.
	 *
	 * This size difference causes problems when the k_spinlock
	 * is embedded into another struct like k_msgq, because C and
	 * C++ will have different ideas on the offsets of the members
	 * that come after the k_spinlock member.
	 *
	 * To prevent this we add a 1 byte dummy member to k_spinlock
	 * when the user selects C++ support and k_spinlock would
	 * otherwise be empty.
	 */
	char dummy;
#endif
};

/* kernel synchronized heap struct */

struct k_heap {
	struct sys_heap heap;
	_wait_q_t wait_q;
	struct k_spinlock lock;
};

#endif /* _ASMLANGUAGE */

#endif /* ZEPHYR_KERNEL_INCLUDE_KERNEL_STRUCTS_H_ */
