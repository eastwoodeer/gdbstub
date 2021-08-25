#ifndef __HEX_H__
#define __HEX_H__

#include <sys/types.h>

typedef unsigned char u8;

int char2hex(char c, u8 *v);
int byte2hex(u8 *b, size_t blen, u8 *hex, size_t hexlen);

#endif /* __HEX_H__ */
