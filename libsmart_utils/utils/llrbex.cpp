#include <sys/mman.h>
#include <stdlib.h>

#include "llrbex.h"

using namespace smart_utils;

//namespace
//{
//	inline uint32_t rb_unused(struct smart_llrb *rb)
//	{
//		return (rb->mask_ + 1) - (rb->in_ - SU_AO(rb->out_));
//	}
//}

//uint64_t smart_llrb::writer_unused_bytes()
//{
//	return (mask_ + 1) - (in_ - SU_AO(out_));
//}

int_fast8_t smart_llrb::init(uint64_t sz, uint32_t esz)
{
	if (sz < 2)
	{
		return -1;
	}

	sz = ROUND_UP(sz, HUGE_PAGE_SIZE);
	SU_ASSERT(0 == (sz & (sz - 1)));
	SU_ASSERT(0 == sz/esz);
	bytes_ = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
	flag_ = false;
	if (bytes_ == MAP_FAILED)
	{
		/* Allocate huge-page-aligned memory to give best chance of allocating
		 * transparent huge-pages.
		 */
		flag_ = true;
		SU_ASSERT(0 == posix_memalign((void**) &bytes_, HUGE_PAGE_SIZE, sz));
	}
	in_ = 0;
	out_ = 0;
	mask_ = sz - 1;
	esize_ = esz;

	return 0;
}

int_fast8_t smart_llrb::destroy()
{
	if (!flag_)
	{
		munmap(bytes_, mask_ + 1);
	}
	else
	{
		free(bytes_);
	}

	bytes_ = NULL;
	in_ = 0;
	out_ = 0;
	mask_ = 0;
	esize_ = 0;

	return 0;
}
//
//byte_t* smart_llrb::writer_get_bytes(uint32_t cnt)
//{
//	uint64_t bytes = cnt * esize_;
//	if (writer_unused_bytes() < bytes)
//	{
//		return NULL;
//	}
//	uint64_t in = in_ & mask_;
//	uint64_t out = SU_AO(out_) & mask_;
//	if ((in > out) && (mask_ + 1 - in) < bytes)
//	{
//		return NULL;
//	}
//
//	smp_wmb();
//
//	return (bytes_ + in);
//}
//
//void smart_llrb::writer_commit_bytes(uint32_t cnt)
//{
//	SU_AO(in_) += (cnt * esize_);
//}

//byte_t* writer_copy_in(struct smart_llrb *rb, const void *src, uint32_t len, uint32_t offset)
//{
//	uint32_t size = rb->mask + 1;
//
//	offset &= rb->mask;
//	if (rb->esize != 1)
//	{
//		offset *= rb->esize;
//		size *= rb->esize;
//		len *= rb->esize;
//	}
//	uint32_t l = min(len, size - offset);
//
//	memcpy(rb->data + offset, src, l);
//	memcpy(rb->data, src + l, len - l);
//	/*
//	 * make sure that the data in the fifo is up to date before
//	 * incrementing the fifo->in index counter
//	 */
//	smp_wmb();
//}
//
//uint32_t rb_writer_in(struct smart_llrb *rb, const void *buf, uint32_t len)
//{
//	uint32_t l = rb_unused(rb);
//	if (len > l)
//		len = l;
//
//	rb_writer_copy_in(rb, buf, len, rb->in);
//	SU_AO(rb->in) += len;
//	return len;
//}

static void rb_reader_copy_out(struct smart_llrb *fifo, void *dst, uint32_t len, uint32_t offset)
{
	uint32_t size = fifo->mask + 1;

	offset &= fifo->mask;
	if (fifo->esize != 1)
	{
		offset *= fifo->esize;
		size *= fifo->esize;
		len *= fifo->esize;
	}
	uint32_t l = min(len, size - offset);

	memcpy(dst, fifo->data + offset, l);
	memcpy(dst + l, fifo->data, len - l);
	/*
	 * make sure that the data is copied before
	 * incrementing the fifo->out index counter
	 */
	smp_wmb();
}

uint32_t rb_reader_out_peek(struct smart_llrb *fifo, void *buf, uint32_t len)
{
	uint32_t l = SU_AO(fifo->in) - fifo->out;
	if (len > l)
		len = l;

	rb_reader_copy_out(fifo, buf, len, fifo->out);
	return len;
}

uint32_t rb_reader_out(struct smart_llrb *fifo, void *buf, uint32_t len)
{
	len = rb_reader_out_peek(fifo, buf, len);
	SU_AO(fifo->out) += len;
	return len;
}

uint32_t rb_max_r(uint32_t len, uint32_t recsize)
{
	uint32_t max = (1 << (recsize << 3)) - 1;

	if (len > max)
		return max;
	return len;
}

#define	RB_READER_PEEK(data, out, mask) \
	((data)[(out) & (mask)])

static uint32_t rb_reader_peek_n(struct smart_llrb *fifo, uint32_t recsize)
{
	uint32_t l = RB_READER_PEEK(fifo->data, fifo->out, fifo->mask);
	if (--recsize)
		l |= RB_READER_PEEK(fifo->data, fifo->out + 1, fifo->mask) << 8;

	return l;
}

#define	__KFIFO_POKE(data, in, mask, val) \
	( \
	(data)[(in) & (mask)] = (unsigned char)(val) \
	)

static void rb_writer_poke_n(struct smart_llrb *fifo, uint32_t n, uint32_t recsize)
{
	__KFIFO_POKE(fifo->data, fifo->in, fifo->mask, n);
	if (recsize > 1)
		__KFIFO_POKE(fifo->data, fifo->in + 1, fifo->mask, n >> 8);
}

uint32_t rb_reader_len_r(struct smart_llrb *fifo, uint32_t recsize)
{
	return rb_reader_peek_n(fifo, recsize);
}

uint32_t rb_writer_in_r(struct smart_llrb *fifo, const void *buf, uint32_t len, uint32_t recsize)
{
	if (len + recsize > rb_unused(fifo))
		return 0;

	rb_writer_poke_n(fifo, len, recsize);

	rb_writer_copy_in(fifo, buf, len, fifo->in + recsize);
	SU_AO(fifo->in) += len + recsize;
	return len;
}

static uint32_t rb_reader_out_copy_r(struct smart_llrb *fifo, void *buf, uint32_t len, uint32_t recsize, uint32_t *n)
{
	*n = rb_reader_peek_n(fifo, recsize);

	if (len > *n)
		len = *n;

	rb_reader_copy_out(fifo, buf, len, fifo->out + recsize);
	return len;
}

uint32_t rb_reader_out_peek_r(struct smart_llrb *fifo, void *buf, uint32_t len, uint32_t recsize)
{
	if (SU_AO(fifo->in) == fifo->out)
		return 0;

	uint32_t n;
	return rb_reader_out_copy_r(fifo, buf, len, recsize, &n);
}

uint32_t rb_reader_out_r(struct smart_llrb *fifo, void *buf, uint32_t len, uint32_t recsize)
{
	if (SU_AO(fifo->in) == fifo->out)
		return 0;

	uint32_t n;
	len = rb_reader_out_copy_r(fifo, buf, len, recsize, &n);
	SU_AO(fifo->out) += n + recsize;
	return len;
}

void rb_reader_skip_r(struct smart_llrb *fifo, uint32_t recsize)
{
	uint32_t n = rb_reader_peek_n(fifo, recsize);
	SU_AO(fifo->out) += n + recsize;
}

