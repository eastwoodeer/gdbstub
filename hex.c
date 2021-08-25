#include "hex.h"

int char2hex(char c, u8 *v)
{
	if (c >= '0' && c <= '9') {
		*v = c - '0';
	} else if (c >= 'a' && c <= 'f') {
		*v = c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		*v = c - 'A' + 10;
	} else {
		return -1;
	}

	return 0;
}
