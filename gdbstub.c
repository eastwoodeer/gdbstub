#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "hex.h"

typedef unsigned char u8;
typedef int i32;

#define error(...)                                                             \
	{                                                                      \
		fprintf(stderr, __VA_ARGS__);                                  \
		fflush(stderr);                                                \
		exit(1);                                                       \
	}

struct gio {
	u8 (*get_char)(void);
	void (*put_char)(u8);
	void (*init)(i32);
};

struct socket_io {
	int fd;
} socket_io;

static int socket_io_init(int fd)
{
	socket_io.fd = fd;
	return 0;
}

static u8 socket_get_char(void)
{
	u8 buf;
	int fd = socket_io.fd;
	int n = recv(fd, &buf, 1, 0);
	if (!n) {
		error("no more contents.\n");
	}

	if (n < 0) {
		error("read failed.");
	}

	return buf;
}

static void socket_put_char(u8 ch)
{
	int fd = socket_io.fd;
	if (send(fd, &ch, 1, 0) < 0) {
		error("send failed.\n");
	}
}

struct gio debug_io = {
	.get_char = socket_get_char,
	.put_char = socket_put_char,
};

static int _get_packet(u8 *buf, size_t buf_len, size_t *len)
{
	u8 ch = '0';
	u8 checksum = 0;

	while (ch != '$') {
		ch = debug_io.get_char();
	}

	*len = 0;
	while (*len < buf_len - 1) {
		ch = debug_io.get_char();

		if (ch == '#') {
			break;
		}

		checksum += ch;

		buf[*len] = ch;
		*len += 1;
	}

	u8 ck1, ck2;
	if (char2hex(debug_io.get_char(), &ck1) ||
	    char2hex(debug_io.get_char(), &ck2)) {
		error("char2hex failed;");
	}
	u8 expected_checksum = (ck1 << 4) + ck2;
	if (checksum != expected_checksum) {
		debug_io.put_char('-');
		error("checksum error, got %d, expected checksum: %d.\n",
		      checksum, expected_checksum);
	}

	debug_io.put_char('+');

	return 0;
}

int main(int argc, char *argv[])
{
	int port = atoi(argv[1]);
	struct sockaddr_in server, client;

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		error("server_fd create failed!\n");
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int optval = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
		   sizeof(optval));

	if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		error("Could not bind socket!\n");
	}

	if (listen(server_fd, 128) < 0) {
		error("Could not listen on socket!\n");
	}

	printf("Server started!\n");

	while (1) {
		socklen_t len = sizeof(client);
		printf("accepting\n");
		int client_fd =
			accept(server_fd, (struct sockaddr *)&client, &len);
		if (client_fd < 0) {
			error("Could not accept new connection.\n");
		}
		printf("accepted\n");
		socket_io_init(client_fd);

		while (1) {
			u8 buf[1024] = { 0 };
			size_t len = 0;
			_get_packet(buf, 1024, &len);
			printf("recv: %s\n", buf);

			/* sprintf(buf, "+$#00"); */

			/* if (send(client_fd, buf, 5, 0) < 0) { */
			/* 	error("send failed.\n"); */
			/* } */
		}
	}

	return 0;
}
