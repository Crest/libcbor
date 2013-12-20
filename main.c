#include "cbor.h"

#include <stdio.h>

void
dump(struct cbor_buf *buf)
{
	for ( size_t i = 0; i < buf->len; i++ ) {
        	printf("%02x ", buf->data[i]);
	}
	putchar('\n');
}

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int128_t i = -((int128_t)(1) << 64);
	int128_t j = 0;

	uint8_t data[100];
	struct cbor_buf buf;
	cbor_buf_init(&buf, data, sizeof(data));
	cbor_add_int128(&buf, i);
	dump(&buf);
	cbor_read_integer(&buf, &j);
	printf(i == j ? "equal\n" : "unequal\n");

	return 0;
}
