/*
 * llrbex.h
 *
 *  Created on: Dec 22, 2015
 *      Author: rock
 */

#ifndef UTILS_LLRBEX_H_
#define UTILS_LLRBEX_H_

#include <stdlib.h>
#include <sys/mman.h>
#include "../base/base.h"

#define MAP_HUGE_2MB    (21 << MAP_HUGE_SHIFT)
#define MAP_HUGE_1GB    (30 << MAP_HUGE_SHIFT)

namespace smart_utils
{
	class smart_llrb
	{
		public:
			smart_llrb()
													: in_(0), mask_(0), bytes_(nullptr), esize_(0), flag_(false), out_(0)
			{
			}
			~smart_llrb()
			{
			}

		public:
			int_fast8_t init(uint64_t sz, uint32_t esz)
			{
				if (sz < HUGE_PAGE_SIZE)
				{
					return -1;
				}

				sz = ROUND_UP(sz, HUGE_PAGE_SIZE);
				SU_ASSERT(0 == (sz & (sz - 1)));
				SU_ASSERT(0 == sz % esz);
				bytes_ = (byte_t*) mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB | MAP_HUGE_1GB | MAP_LOCKED, -1, 0);
				flag_ = false;
				if (bytes_ == MAP_FAILED)
				{
					/* Allocate huge-page-aligned memory to give best chance of allocating
					 * transparent huge-pages.
					 */
					flag_ = true;
					SU_ASSERT(0 == posix_memalign((void**) &bytes_, HUGE_PAGE_SIZE, sz));
					mlock(bytes_, sz);
				}
				in_ = 0;
				out_ = 0;
				mask_ = sz - 1;
				esize_ = esz;

				return 0;
			}

			int_fast8_t destroy()
			{
				if (!flag_)
				{
					munmap(bytes_, mask_ + 1);
				}
				else
				{
					munlock(bytes_, mask_ + 1);
					free(bytes_);
				}

				bytes_ = NULL;
				in_ = 0;
				out_ = 0;
				mask_ = 0;
				esize_ = 0;

				return 0;
			}
			///
			inline uint64_t reader_unused_datas()
			{
				return (SU_AO(in_) - out_);

			}
			inline uint64_t writer_unused_bytes()
			{
				return (mask_ + 1) - (in_ - SU_AO(out_));
			}
			///
			inline byte_t* writer_get_bytes(uint32_t cnt)
			{
				uint64_t bytes = cnt * esize_;
				if (writer_unused_bytes() < bytes)
				{
					return NULL;
				}
				uint64_t in = in_ & mask_;
				uint64_t out = SU_AO(out_) & mask_;
				if ((in > out) && ((mask_ + 1 - in) < bytes))
				{
					return NULL;
				}

				return (bytes_ + in);
			}
			inline void writer_commit_bytes(uint32_t cnt)
			{
				smp_wmb();
				SU_AO(in_) += (cnt * esize_);
			}
			////
			inline byte_t* reader_get_data(uint32_t cnt)
			{
				uint64_t bytes = cnt * esize_;
				if (reader_unused_datas() < bytes)
				{
					return NULL;
				}

				uint64_t in = in_ & mask_;
				uint64_t out = out_ & mask_;
				if ((out > in) && ((mask_ + 1 - out) < bytes))
				{
					return NULL;
				}

				return (bytes_ + out);
			}

			inline void reader_commit_data(uint32_t cnt)
			{
				smp_wmb();
				SU_AO(out_) += (cnt * esize_);
			}

		private:
			uint64_t in_;
			uint64_t mask_;
			byte_t* bytes_;
			uint32_t esize_;
			bool flag_;
			uint64_t out_ __attribute__ ((aligned (64)));
	};
}

#endif /* UTILS_LLRBEX_H_ */
