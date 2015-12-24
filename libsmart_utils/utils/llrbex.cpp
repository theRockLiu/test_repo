#include "llrbex.h"

/*
 * internal helper to calculate the unused elements in a fifo
 */
static inline uint32_t rb_unused(struct smart_llrb *fifo)
{
	return (fifo->mask + 1) - (fifo->in - SUAO(fifo->out));
}

int rb_alloc(struct smart_llrb *fifo, uint32_t size, uint32_t esize)
{
	/*
	 * round down to the next power of 2, since our 'let the indices
	 * wrap' technique works only in this case.
	 */
	size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;

	if (size < 2)
	{
		fifo->data = NULL;
		fifo->mask = 0;
		return -1;
	}

	fifo->data = malloc(size * esize);

	if (!fifo->data)
	{
		fifo->mask = 0;
		return -2;
	}
	fifo->mask = size - 1;

	smp_wmb();

	return 0;
}

void rb_free(struct smart_llrb *fifo)
{
	free(fifo->data);
	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = 0;
	fifo->data = NULL;
	fifo->mask = 0;

	smp_wmb();
}

int rb_init(struct smart_llrb *fifo, void *buffer, uint32_t size, uint32_t esize)
{
	size /= esize;

	size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;
	fifo->data = buffer;

	if (size < 2)
	{
		fifo->mask = 0;
		return -1;
	}
	fifo->mask = size - 1;

	smp_wmb();

	return 0;
}

static void rb_writer_copy_in(struct smart_llrb *rb, const void *src, uint32_t len, uint32_t offset)
{
	uint32_t size = rb->mask + 1;

	offset &= rb->mask;
	if (rb->esize != 1)
	{
		offset *= rb->esize;
		size *= rb->esize;
		len *= rb->esize;
	}
	uint32_t l = min(len, size - offset);

	memcpy(rb->data + offset, src, l);
	memcpy(rb->data, src + l, len - l);
	/*
	 * make sure that the data in the fifo is up to date before
	 * incrementing the fifo->in index counter
	 */
	smp_wmb();
}

uint32_t rb_writer_in(struct smart_llrb *rb, const void *buf, uint32_t len)
{
	uint32_t l = rb_unused(rb);
	if (len > l)
		len = l;

	rb_writer_copy_in(rb, buf, len, rb->in);
	SUAO(rb->in) += len;
	return len;
}

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
	uint32_t l = SUAO(fifo->in) - fifo->out;
	if (len > l)
		len = l;

	rb_reader_copy_out(fifo, buf, len, fifo->out);
	return len;
}

uint32_t rb_reader_out(struct smart_llrb *fifo, void *buf, uint32_t len)
{
	len = rb_reader_out_peek(fifo, buf, len);
	SUAO(fifo->out) += len;
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
	SUAO(fifo->in) += len + recsize;
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
	if (SUAO(fifo->in) == fifo->out)
		return 0;

	uint32_t n;
	return rb_reader_out_copy_r(fifo, buf, len, recsize, &n);
}

uint32_t rb_reader_out_r(struct smart_llrb *fifo, void *buf, uint32_t len, uint32_t recsize)
{
	if (SUAO(fifo->in) == fifo->out)
		return 0;

	uint32_t n;
	len = rb_reader_out_copy_r(fifo, buf, len, recsize, &n);
	SUAO(fifo->out) += n + recsize;
	return len;
}

void rb_reader_skip_r(struct smart_llrb *fifo, uint32_t recsize)
{
	uint32_t n = rb_reader_peek_n(fifo, recsize);
	SUAO(fifo->out) += n + recsize;
}

