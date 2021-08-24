#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define error(...)                                                             \
	{                                                                      \
		fprintf(stderr, __VA_ARGS__);                                  \
		fflush(stderr);                                                \
		exit(1);                                                       \
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

		while (1) {
			char buf[1024] = { 0 };
			int n = recv(client_fd, buf, 1024, 0);
			if (!n) {
				break;
			}

			if (n < 0) {
				error("read failed.");
			}

			printf("recv: %s\n", buf);

			sprintf(buf, "%c%c", '+', '\0');

			if (send(client_fd, buf, 2, 0) < 0) {
				error("send failed.\n");
			}
		}
	}

	return 0;
}
