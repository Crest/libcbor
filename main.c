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
	double x = 23.42;
	double y;

	uint8_t data[100];
	struct cbor_buf buf;
	cbor_buf_init_empty(&buf, data, sizeof(data));
	cbor_add_double(&buf, x);
	dump(&buf);
	cbor_read_double(&buf, &y);
	printf("%f\n", y);

	return 0;
}
