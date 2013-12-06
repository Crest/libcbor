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

struct cbor_buf {
	uint8_t *data;
	size_t   len;
	size_t   cap;
};

static inline void
cbor_buf_init(struct cbor_buf *buf, void *data, size_t size)
{
	buf->data = data;
	buf->len  = 0;
	buf->cap  = size;
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
	buf->len  = len + sizeof(m) + sizeof(n);

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

#endif /* LIBCBOR_CBOR_H */
