//============================================================================
// Name        : llrb.h
// Author      : TheRockLiuHY
// Date        : Jun 10, 2015
// Copyright   : liuhongyang's copyright
// Description : smart_utils
//============================================================================

#ifndef UTILS_LLRB_H_
#define UTILS_LLRB_H_

#include <stdlib.h>
#include <sys/mman.h>
#include "../base/base.h"



static inline unsigned int roundup_pow_of_two(unsigned int x)
{
	x--;
	x |= x >> 1;  // handle  2 bit numbers
	x |= x >> 2;// handle  4 bit numbers
	x |= x >> 4;// handle  8 bit numbers
	x |= x >> 8;// handle 16 bit numbers
	x |= x >> 16;// handle 32 bit numbers
	x++;

	return x;
}

namespace smart_utils
{
	///heap is ok, not stack, you know why:).
	class llrb
	{
	public:
		llrb(): in_idx_(0), out_idx_(0),rb_(NULL), mask_(0), elem_size_(0){}
		~llrb()
		{
			if (NULL != rb_)
			{
				munlock(rb_, elem_size_ * (mask_ + 1));
				free(rb_);
			}
		}

	public:
		int32_t init(uint32_t es, uint32_t cnt)
		{
			in_idx_ = out_idx_ = 0;
			elem_size_ = es;
			if (elem_size_ == 0)
			{
				return -1;
			}
			cnt = roundup_pow_of_two(cnt);
			if (cnt < 2)
			{
				return -1;
			}
			mask_ = cnt - 1;
			//int32_t ret = posix_memalign((void**)&rb_, __MEM_PAGE_SIZE__, es * cnt); //todo: mlock, huge page...
			if (0 != posix_memalign((void**)&rb_, __MEM_PAGE_SIZE__, es * cnt))
			{
				return -1;
			}
			if (0 != mlock(rb_, es * cnt))
			{
				return -1;
			}

			return 0;
		}

		void reset_idx(){in_idx_ = out_idx_ = 0;}

		inline uint64_t get_elem_cnt() {return mask_ + 1;}
		inline uint64_t get_elem_size() {return elem_size_;}
		inline uint64_t get_data_cnt() {return (in_idx_ - out_idx_);}
		inline uint64_t get_free_cnt() {return get_elem_cnt() - get_data_cnt();}
		inline uint64_t get_in_idx(){return in_idx_;}
		inline uint64_t get_out_idx() {return out_idx_;}

		inline byte_t*  get_next_free() {return get_free_cnt() == 0 ? NULL : rb_ + (in_idx_ & mask_) * elem_size_;}
		inline uint64_t get_next_free_cnt() {return get_free_cnt() == 0 ? 0 : ((in_idx_ & mask_) > (out_idx_ & mask_) ? (get_elem_cnt() - (in_idx_ & mask_)) : ((out_idx_ & mask_) - (in_idx_ & mask_)));}
		inline byte_t*  get_data(uint64_t idx) {return  get_data_cnt() == 0 ? NULL : ((idx < out_idx_ || idx >= in_idx_) ? NULL : rb_+(idx & mask_) * elem_size_);}
		inline void inc_in_idx(uint64_t cnt) {smp_wmb(); in_idx_ += cnt;}
		inline void inc_out_idx(uint64_t cnt) {smp_wmb(); out_idx_ += cnt;}

	private:
		//alignas(__CLS__)c
		uint64_t in_idx_ __attribute__((aligned(__CLS__)));
		uint64_t out_idx_;
		byte_t *rb_;
		uint64_t mask_;
		uint64_t elem_size_;
	};

} /* namespace ns_smart_utils */

#endif /* UTILS_LLRB_H_ */
