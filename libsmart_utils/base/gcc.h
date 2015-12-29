/*
 * gcc.h
 *
 *  Created on: Dec 29, 2015
 *      Author: rock
 */

#ifndef BASE_GCC_H_
#define BASE_GCC_H_

/* At what version was this introduced? */
#if __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ > 91)
# define SU_LIKELY(t)    __builtin_expect((t), 1)
# define SU_UNLIKELY(t)  __builtin_expect((t), 0)
#endif

#if __GNUC__ >= 4 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
# define su_noinline  static __attribute__((__noinline__))
/* (Linux 2.6 defines its own "noinline", so we use the "__noinline__" form) */
#else
# define su_noinline  static
#endif

#define SU_ALIGN(x) __attribute__ ((aligned (x)))
#define SU_UNUSED __attribute__((__unused__))

#endif /* BASE_GCC_H_ */
