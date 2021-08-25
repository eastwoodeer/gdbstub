#include "hex.h"

int char2hex(char c, u8 *hex)
{
	if (c >= '0' && c <= '9') {
		*hex = c - '0';
	} else if (c >= 'a' && c <= 'f') {
		*hex = c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		*hex = c - 'A' + 10;
	} else {
		return -1;
	}

	return 0;
}

int hex2char(u8 hex, u8 *c)
{
	if (hex <= 9) {
		*c = '0' + hex;
	} else if (hex <= 0xf) {
		*c = 'a' + hex - 10;
	} else {
		return -1;
	}

	return 0;
}

int byte2hex(u8 *b, size_t blen, u8 *hex, size_t hexlen)
{
	/* 1 byte 102 -> hex 0x66 -> 66 */
	if (hexlen < 2 * blen) {
		return -1;
	}

	for (size_t i = 0; i < blen; i++) {
		if (hex2char(b[i] >> 4, &hex[2 * i])) {
			return -1;
		}
		if (hex2char(b[i] & 0xf, &hex[2 * i + 1])) {
			return -1;
		}
	}

	return 0;
}
