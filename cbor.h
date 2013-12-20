/**
 * Copyright (c) 2013, Jan Bramkamp
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#ifndef LIBCBOR_CBOR_H
#define LIBCBOR_CBOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

typedef __int128_t  int128_t;
typedef __uint128_t uint128_;

struct cbor_buf {
	uint8_t *data;
	size_t   len;
	size_t   cap;
	size_t   idx;
};

static inline void
cbor_buf_init(struct cbor_buf *buf, void *data, size_t size)
{
	buf->data = data;
	buf->len  = 0;
	buf->cap  = size;
	buf->idx  = 0;
}

static inline size_t
cbor_buf_length(struct cbor_buf *buf)
{
	return buf->len;
}

static inline size_t
cbor_buf_capacity(struct cbor_buf *buf)
{
	return buf->cap;
}

static inline void *
cbor_buf_data(struct cbor_buf *buf)
{
	return buf->data;
}

static inline size_t
cbor_buf_space(struct cbor_buf *buf)
{
	return buf->cap - buf->len;
}

static inline bool
cbor_buf_seek_fwd(struct cbor_buf *buf, size_t delta)
{
	size_t len = buf->len;
	size_t idx = buf->idx + delta;

	if ( idx > len ) {
		return false;
	}
	buf->idx = idx;

	return true;
}

static inline bool
cbor_buf_seek_rev(struct cbor_buf *buf, size_t delta)
{
	size_t idx = buf->idx;

	if ( delta > idx ) {
		return false;
	}
	buf->idx = idx - delta;

	return true;
}

static inline bool
cbor_buf_seek_set(struct cbor_buf *buf, size_t idx)
{
	size_t len = buf->len;
	if ( idx > len ) {
		return false;
	}
	buf->idx = idx;

	return true;
}

static inline size_t
cbor_buf_index(struct cbor_buf *buf)
{
	return buf->idx;
}

static inline bool
cbor_buf_append_byte(struct cbor_buf *buf, uint8_t n)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(n) ) {
		return false;
	}

	data[len] = n;
	buf->len  = len + sizeof(n);

	return true;
}

static inline bool
cbor_buf_append_byte_plus(struct cbor_buf *buf, uint8_t n, void *plus, size_t size)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(n) + size ) {
        	return false;
	}

	data[len] = n;
	memcpy(data + len + 1, plus, size);
	buf->len = len + sizeof(n) + size;

	return true;
}

static inline bool
cbor_buf_append_2byte(struct cbor_buf *buf, uint8_t m, uint8_t n)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) ) { 
        	return false;
	}

	data[len    ] = m;
	data[len + 1] = n;
	buf->len      = len + sizeof(m) + sizeof(n);

	return true;

}

static inline bool
cbor_buf_append_2byte_plus(struct cbor_buf *buf, uint8_t m, uint8_t n, void *plus, size_t size)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) + size ) {
        	return false;
	}

	data[len    ] = m;
	data[len + 1] = n;
	memcpy(data + len + 2, plus, size);
	buf->len      = len + sizeof(m) + sizeof(n) + size;

	return true;
}

static inline bool
cbor_buf_append_3byte(struct cbor_buf *buf, uint8_t m, uint16_t n)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) ) { 
        	return false;
	}

	data[len    ] = m;
	data[len + 1] = (uint8_t)(n >> 8);
	data[len + 2] = (uint8_t)(n     );
	buf->len      = len + sizeof(m) + sizeof(n);

	return true;
}

static inline bool
cbor_buf_append_3byte_plus(struct cbor_buf *buf, uint8_t m, uint16_t n, void *plus, size_t size)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) + size ) {
        	return false;
	}

	data[len    ] = m;
	data[len + 1] = (uint8_t)(n >> 8);
	data[len + 2] = (uint8_t)(n     );
	memcpy(data + len + 3, plus, size);
	buf->len      = len + sizeof(m) + sizeof(n) + size;

	return true;
}

static inline bool
cbor_buf_append_5byte(struct cbor_buf *buf, uint8_t m, uint32_t n)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) ) { 
        	return false;
	}

	data[len    ] = m;
	data[len + 1] = (uint8_t)(n >> 24);
	data[len + 2] = (uint8_t)(n >> 16);
	data[len + 3] = (uint8_t)(n >>  8);
	data[len + 4] = (uint8_t)(n      );
	buf->len  = len + sizeof(m) + sizeof(n);

	return true;
}

static inline bool
cbor_buf_append_5byte_plus(struct cbor_buf *buf, uint8_t m, uint32_t n, void *plus, size_t size)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) + size ) {
		return false;
	}

	data[len    ] = m;
	data[len + 1] = (uint8_t)(n >> 24);
	data[len + 2] = (uint8_t)(n >> 16);
	data[len + 3] = (uint8_t)(n >>  8);
	data[len + 4] = (uint8_t)(n      );
	memcpy(data + len + 5, plus, size);
	buf->len      = len + sizeof(m) + sizeof(n) + size;

	return true;
}

static inline bool
cbor_buf_append_9byte(struct cbor_buf *buf, uint8_t m, uint64_t n)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) ) {
        	return false;
	}

	data[len    ] = m;
	data[len + 1] = (uint8_t)(n >> 56);
	data[len + 2] = (uint8_t)(n >> 48);
	data[len + 3] = (uint8_t)(n >> 40);
	data[len + 4] = (uint8_t)(n >> 32);
	data[len + 5] = (uint8_t)(n >> 24);
	data[len + 6] = (uint8_t)(n >> 16);
	data[len + 7] = (uint8_t)(n >>  8);
	data[len + 8] = (uint8_t)(n      );
	buf->len      = len + sizeof(m) + sizeof(n);

	return true;
}

static inline bool
cbor_buf_append_9byte_plus(struct cbor_buf *buf, uint8_t m, uint64_t n, void *plus, size_t size)
{
	uint8_t *data = buf->data;
	size_t   len  = buf->len;
	size_t   cap  = buf->cap;

	if ( cap - len < sizeof(m) + sizeof(n) + size ) {
		return false;
	}

	data[len    ] = m;
	data[len + 1] = (uint8_t)(n >> 56);
	data[len + 2] = (uint8_t)(n >> 48);
	data[len + 3] = (uint8_t)(n >> 40);
	data[len + 4] = (uint8_t)(n >> 32);
	data[len + 5] = (uint8_t)(n >> 24);
	data[len + 6] = (uint8_t)(n >> 16);
	data[len + 7] = (uint8_t)(n >>  8);
	data[len + 8] = (uint8_t)(n      );
	memcpy(data + len + 9, plus, size);
	buf->len      = len + sizeof(m) + sizeof(n) + size;

	return true;
}

static inline bool
cbor_add_false(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0xf4);
}

static inline bool
cbor_add_true(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0xf5);
}

static inline bool
cbor_add_null(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0xf6);
}

static inline bool
cbor_add_undef(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0xf7);
}

static inline bool
cbor_add_float(struct cbor_buf *buf, float x)
{
        union {
		uint32_t n;
		float    x;
	} tmp = { .x = x };
	return cbor_buf_append_5byte(buf, 0xfa, tmp.n);
}

static inline bool
cbor_add_double(struct cbor_buf *buf, double x)
{
        union {
		uint64_t n;
		double   x;
	} tmp = { .x = x };
	return cbor_buf_append_9byte(buf, 0xfb, tmp.n);
}

static inline bool
cbor_add_break(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0xff);
}

static inline bool
cbor_add_uint64(struct cbor_buf *buf, uint64_t n)
{
	if ( n <= 23 ) {
        	return cbor_buf_append_byte(buf, (uint8_t)n);
	}

	if ( n <= UINT8_MAX ) {
        	return cbor_buf_append_2byte(buf, 0x18, (uint8_t)n);
	}

	if ( n <= UINT16_MAX ) {
        	return cbor_buf_append_3byte(buf, 0x19, (uint16_t)n);
	}

	if ( n <= UINT32_MAX ) {
        	return cbor_buf_append_5byte(buf, 0x1a, (uint32_t)n);
	}

	return cbor_buf_append_9byte(buf, 0x1b, n);
}

static inline bool
cbor_add_int64(struct cbor_buf *buf, int64_t i)
{
	if ( i >= 0 ) {
		return cbor_add_uint64(buf, (uint64_t)i);
	}

        uint64_t n = ((uint64_t)(-i))-1;

	if ( n <= 23 ) {
        	return cbor_buf_append_byte(buf, (uint8_t)(0x20 | n));
	}

	if ( n <= UINT8_MAX ) {
        	return cbor_buf_append_2byte(buf, 0x38, (uint8_t)n);
	}

	if ( n <= UINT16_MAX ) {
        	return cbor_buf_append_3byte(buf, 0x39, (uint16_t)n);
	}

	if ( n <= UINT32_MAX ) {
        	return cbor_buf_append_5byte(buf, 0x3a, (uint32_t)n);
	}

	return cbor_buf_append_9byte(buf, 0x3b, n);
}

static inline bool
cbor_add_int128(struct cbor_buf *buf, int128_t i)
{
	int128_t max = (int128_t)UINT64_MAX;
	int128_t min = -max - 1;
	
	if ( i > max || i < min ) {
		return false;
	}
	if ( i >= 0 ) {
		return cbor_add_uint64(buf, (uint64_t)i);
	}

	uint64_t n = (uint64_t)(-i - 1);

	if ( n <= 23 ) {
        	return cbor_buf_append_byte(buf, (uint8_t)(0x20 | n));
	}

	if ( n <= UINT8_MAX ) {
        	return cbor_buf_append_2byte(buf, 0x38, (uint8_t)n);
	}

	if ( n <= UINT16_MAX ) {
        	return cbor_buf_append_3byte(buf, 0x39, (uint16_t)n);
	}

	if ( n <= UINT32_MAX ) {
        	return cbor_buf_append_5byte(buf, 0x3a, (uint32_t)n);
	}

	return cbor_buf_append_9byte(buf, 0x3b, n);

}

static inline bool
cbor_add_byte_str(struct cbor_buf *buf, void *data, size_t len)
{
	if ( len <= 23 ) {
                return cbor_buf_append_byte_plus(buf, (uint8_t)(0x40 | len), data, len);
	}

	if ( len <= UINT8_MAX ) {
        	return cbor_buf_append_2byte_plus(buf, 0x58, (uint8_t)len, data, len);
	}

	if ( len <= UINT16_MAX ) {
		return cbor_buf_append_3byte_plus(buf, 0x59, (uint16_t)len, data, len);
	}

	if ( len <= UINT32_MAX ) {
		return cbor_buf_append_5byte_plus(buf, 0x5a, (uint32_t)len, data, len);
	}

	return cbor_buf_append_9byte_plus(buf, 0x5b, len, data, len);
}

static inline bool
cbor_add_utf8_str(struct cbor_buf *buf, char *data, size_t len)
{
	if ( len <= 23 ) {
                return cbor_buf_append_byte_plus(buf, (uint8_t)(0x60 | len), data, len);
	}

	if ( len <= UINT8_MAX ) {
        	return cbor_buf_append_2byte_plus(buf, 0x78, (uint8_t)len, data, len);
	}

	if ( len <= UINT16_MAX ) {
		return cbor_buf_append_3byte_plus(buf, 0x79, (uint16_t)len, data, len);
	}

	if ( len <= UINT32_MAX ) {
		return cbor_buf_append_5byte_plus(buf, 0x7a, (uint32_t)len, data, len);
	}

	return cbor_buf_append_9byte_plus(buf, 0x7b, len, data, len);
}

static inline bool
cbor_add_utf8_cstr(struct cbor_buf *buf, char *str)
{
	size_t len = strlen(str);
	
	return cbor_add_utf8_str(buf, str, len);
}

static inline bool
cbor_add_array(struct cbor_buf *buf, uint64_t size)
{
	if ( size <= 23 ) {
		return cbor_buf_append_byte(buf, (uint8_t)(0x80 | size));
	}

	if ( size < UINT8_MAX ) {
		return cbor_buf_append_2byte(buf, 0x98, (uint8_t)size);
	}

	if ( size < UINT16_MAX ) {
		return cbor_buf_append_3byte(buf, 0x99, (uint16_t)size);
	}

	if ( size < UINT32_MAX ) {
		return cbor_buf_append_5byte(buf, 0x9a, (uint32_t)size);
	}

	return cbor_buf_append_9byte(buf, 0x9b, size);
}

static inline bool
cbor_add_array_start(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0x9f);
}

#define cbor_add_array_end cbor_add_break

static inline bool
cbor_add_map(struct cbor_buf *buf, uint64_t size)
{
	if ( size <= 23 ) {
		return cbor_buf_append_byte(buf, (uint8_t)(0xa0 | size));
	}

	if ( size < UINT8_MAX ) {
		return cbor_buf_append_2byte(buf, 0xb8, (uint8_t)size);
	}

	if ( size < UINT16_MAX ) {
		return cbor_buf_append_3byte(buf, 0xb9, (uint16_t)size);
	}

	if ( size < UINT32_MAX ) {
		return cbor_buf_append_5byte(buf, 0xba, (uint32_t)size);
	}

	return cbor_buf_append_9byte(buf, 0xbb, size);
}

static inline bool
cbor_add_map_start(struct cbor_buf *buf)
{
	return cbor_buf_append_byte(buf, 0xbf);
}

#define cbor_add_map_end cbor_add_break

static inline bool
cbor_is_positive_integer(struct cbor_buf *buf)
{
	uint8_t *data = buf->data;
	size_t   idx  = buf->idx;
	uint8_t  byte = data[idx];

	return byte < 0x20;
}

static inline bool
cbor_is_negative_integer(struct cbor_buf *buf)
{
	uint8_t *data = buf->data;
	size_t   idx  = buf->idx;
	uint8_t  byte = data[idx];

	return byte >= 0x20 && byte < 0x40;
}

static inline bool
cbor_is_integer(struct cbor_buf *buf)
{
	uint8_t *data = buf->data;
	size_t   idx  = buf->idx;
	uint8_t  byte = data[idx];

	return byte < 0x40;
}

// TODO: Find a better portable way to express unaligned loads.
static inline uint64_t
cbor_peek_u8(uint8_t *data)
{
	return (uint64_t)data[0];
}

static inline uint64_t
cbor_peek_u16(uint8_t *data)
{
	uint16_t value = ((uint16_t)data[0]) << 8 |
	                 ((uint16_t)data[1]); 

	return (uint64_t)value;
}

static inline uint64_t
cbor_peek_u32(uint8_t *data)
{
	uint32_t value = ((uint32_t)data[0]) << 24 |
	                 ((uint32_t)data[1]) << 16 |
			 ((uint32_t)data[2]) <<  8 |
			 ((uint32_t)data[3]);

	return (uint64_t)value;
}

static inline uint64_t
cbor_peek_u64(uint8_t *data)
{
	uint64_t value = ((uint64_t)data[0]) << 56 |
	                 ((uint64_t)data[1]) << 48 |
			 ((uint64_t)data[2]) << 40 |
			 ((uint64_t)data[3]) << 32 |
			 ((uint64_t)data[4]) << 24 |
			 ((uint64_t)data[5]) << 16 |
			 ((uint64_t)data[6]) <<  8 |
			 ((uint64_t)data[7]);

	return value;
}

static inline bool
cbor_read_positive_integer(struct cbor_buf *buf, uint64_t *value)
{
	size_t   len  = buf->len;
	size_t   idx  = buf->idx;
	uint8_t *data = buf->data + idx;

	if ( idx >= len ) {
		return false;
	}

	uint8_t byte = data[0];
	
	if ( byte < 0x18 ) {
        	buf->idx = idx + sizeof(byte);
		*value   = (uint64_t)byte;
		return true;
	}
        
	data += sizeof(byte);
	switch ( byte ) {
        	case 0x18:
			if ( len - idx < sizeof(byte) + sizeof(uint8_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint8_t);
			*value   = cbor_peek_u8(data);
			return true;

		case 0x19:
			if ( len - idx < sizeof(byte) + sizeof(uint16_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint16_t);
			*value   = cbor_peek_u16(data);
			return true;

		case 0x1a:
			if ( len - idx < sizeof(byte) + sizeof(uint32_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint32_t);
			*value   = cbor_peek_u32(data);
			return true;

		case 0x1b:
			if ( len - idx < sizeof(byte) + sizeof(uint64_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint64_t);
			*value   = cbor_peek_u64(data);
			return true;

		default:
			return false;
	}

	return false;
}

static inline bool
cbor_read_negative_integer_biased(struct cbor_buf *buf, uint64_t *value)
{
	size_t   len  = buf->len;
	size_t   idx  = buf->idx;
	uint8_t *data = buf->data + idx;

	if ( idx >= len ) {
		return false;
	}

	uint8_t  byte = data[idx];

	if ( byte >= 0x20 && byte < 0x38 ) {
        	buf->idx = idx + sizeof(byte);
		*value   = (uint64_t)byte - 0x20;
		return true;
	}

	data += sizeof(byte);
	switch ( byte ) {
        	case 0x38:
			if ( len - idx < sizeof(byte) + sizeof(uint8_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint8_t);
			*value   = cbor_peek_u8(data);
			return true;

		case 0x39:
			if ( len - idx < sizeof(byte) + sizeof(uint16_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint16_t);
			*value   = cbor_peek_u16(data);
			return true;

		case 0x3a:
			if ( len - idx < sizeof(byte) + sizeof(uint32_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint32_t);
			*value   = cbor_peek_u32(data);
			return true;

		case 0x3b:
			if ( len - idx < sizeof(byte) + sizeof(uint64_t) ) {
				return false;
			}
			buf->idx = idx + sizeof(byte) + sizeof(uint64_t);
			*value   = cbor_peek_u64(data);
			return true;

		default:
			return false;
	}

	return false;
}

static inline bool
cbor_read_negative_integer_unbiased(struct cbor_buf *buf, int128_t *value)
{
	uint64_t biased;
	int128_t unbiased;

	if ( cbor_read_negative_integer_biased(buf, &biased) == false ) {
        	return false;
	}

	unbiased = (-(int128_t)biased) - 1;
	*value = unbiased;

	return true;
}

static inline bool
cbor_read_integer(struct cbor_buf *buf, int128_t *value)
{
	size_t   len  = buf->len;
	size_t   idx  = buf->idx;
	uint8_t *data = buf->data + idx;

	if ( idx >= len ) {
		return false;
	}

	uint8_t byte = data[0];

	if ( byte < 0x20 ) {
                uint64_t positive_value;
		if ( !cbor_read_positive_integer(buf, &positive_value) ) {
			return false;
		}
		*value = (int128_t)positive_value;
		return true;
	}
	if ( byte < 0x40 ) {
        	return cbor_read_negative_integer_unbiased(buf, value);
	}

	return false;
}


#endif /* LIBCBOR_CBOR_H */
