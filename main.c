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

	uint8_t data[100];
	struct cbor_buf buf;
	cbor_buf_init(&buf, data, sizeof(data));
	cbor_add_map_start(&buf);
	cbor_add_utf8_cstr(&buf, "foo");
	cbor_add_utf8_cstr(&buf, "bar");
	cbor_add_map_end(&buf);
	dump(&buf);

	return 0;
}
