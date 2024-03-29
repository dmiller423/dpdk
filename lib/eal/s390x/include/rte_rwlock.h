/* SPDX-License-Identifier: BSD-3-Clause
 * (c) Copyright IBM Corp. 2018, 2019
 */

#ifndef _RTE_RWLOCK_S390X_H_
#define _RTE_RWLOCK_S390X_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "generic/rte_rwlock.h"

static inline void
rte_rwlock_read_lock_tm(rte_rwlock_t *rwl)
{
	rte_rwlock_read_lock(rwl);
}

static inline void
rte_rwlock_read_unlock_tm(rte_rwlock_t *rwl)
{
	rte_rwlock_read_unlock(rwl);
}

static inline void
rte_rwlock_write_lock_tm(rte_rwlock_t *rwl)
{
	rte_rwlock_write_lock(rwl);
}

static inline void
rte_rwlock_write_unlock_tm(rte_rwlock_t *rwl)
{
	rte_rwlock_write_unlock(rwl);
}

#ifdef __cplusplus
}
#endif

#endif /* _RTE_RWLOCK_S390X_H_ */
