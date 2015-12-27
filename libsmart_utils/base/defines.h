//============================================================================
// Name        : Defines.h
// Author      : TheRockLiuHY
// Date        : Apr 29, 2015
// Copyright   : liuhongyang's copyright
// Description : SmartUtils
//============================================================================

#ifndef UTILS_DEFINES_H_
#define UTILS_DEFINES_H_

#include <cassert> ///for c++ assert

#ifdef NDEBUG
#define SU_CHECK(expr) assert(expr); ///to do ...
#else
#define SU_CHECK(expr) assert(expr);
#endif

#define SU_CHECK_FAIL_RET(expr, ret) \
	if (!(expr)) \
	{\
		return ret; \
	}

#define DISABLE_CONSTRUCT_AND_DESTRUCT(class_name) \
	class_name () = delete; \
	~class_name () = delete;

#define DISABLE_COPY(class_name) \
	class_name ( class_name &) = delete; \
	class_name & operator= (class_name & ) = delete;

#define DISABLE_MOVE(class_name) \
	class_name ( class_name && ) = delete; \
	class_name & operator= ( class_name && ) = delete;

#define SAFE_CLOSE_FD(fd) \
		if (-1 != fd) \
		{\
			::close(fd);\
			fd = -1;\
		}

#define __CLS__ (64)

//#define smp_wmb() asm __volatile__("" ::: "memory")

#define __MEM_PAGE_SIZE__ (4096)

#define min(x, y) ((x) < (y) ? (x) : (y))
//#define min(x, y) ({				\
//	typeof(x) _min1 = (x);			\
//	typeof(y) _min2 = (y);			\
//	(void) (&_min1 == &_min2);		\
//	_min1 < _min2 ? _min1 : _min2; })

/* The "volatile" is due to gcc bugs */
#define smp_wmb() __asm__ __volatile__("": : :"memory")
#define SU_AO(x) (*(volatile typeof(x) *)&(x))


#ifndef MAP_HUGETLB
/* Not always defined in glibc headers.  If the running kernel does not
 * understand this flag it will ignore it and you may not get huge pages.
 * (In that case ef_memreg_alloc() may fail when using packed-stream mode).
 */
# define MAP_HUGETLB  0x40000
#endif


#ifdef __PPC__
# define HUGE_PAGE_SIZE    (16ll * 1024 * 1024)
#elif defined(__x86_64__) || defined(__i386__)
# define HUGE_PAGE_SIZE    (2ll * 1024 * 1024)
#else
# error "Please define huge_page_size"
#endif

#define ROUND_UP(p, align)   (((p)+(align)-1u) & ~((align)-1u))



#endif /* UTILS_DEFINES_H_ */
