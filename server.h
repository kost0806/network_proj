#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define true 1
#define BUF_SIZE 1024

void init(unsigned int port);
int communicate(int client_fd);
int check_file(char *path);
char *get_status(int code);
char *get_mime(char *format);
void print_help(char *path);
int header_len(char *status, char *connection, char *mime);
char *create_header(char *header, char *status, char *connection, char *mime);

static char *DOCUMENT_ROOT = "/var/my_www";
static char *header = "HTTP/1.1 %s\r\n"
											"Connection: %s\r\n"
											"Content-Type: %s\r\n"
											"\r\n";
static char *NOT_FOUND = 	"<!DOCTYPE HTML>"
													"<html>"
													"	<head></head>"
													"	<body>"
													"		<h1>Not Found</h1>"
													"		<p>The requested URL %s was not found on this server.</p>"
													"	</body>"
													"</html>";

static char *FORBIDDEN = 	"<!DOCTYPE HTML>"
													"<html>"
													"	<head></head>"
													"	<body>"
													"		<h1>Forbidden</h1>"
													"		<p>You don't have permission to access %s on this server.</p>"
													"	</body>"
													"</html>";


int count = 0;

void init(unsigned int port) {
	int soc_fd, client_fd;

	struct sockaddr_in server;
	struct sockaddr_in client;
	int listen_ret;
	int client_addr_size;

	soc_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (soc_fd == -1) {
		fprintf(stderr, "Socket create error\n");
		exit(3); // socket create error
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(soc_fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "Socket bind fail\n");
		exit(4); // bind fail
	}

	if (listen(soc_fd, 5) == -1) {
		fprintf(stderr, "Socket listen fail\n");
		exit(5); // listening fail
	}

	while (true) {
		client_addr_size = sizeof(struct sockaddr);
		if ((client_fd = accept(soc_fd, (struct sockaddr*)&client, &client_addr_size)) == -1) {
			fprintf(stderr, "Socket accept error\n");
			exit(6); // accepting fail
		}

		communicate(client_fd);
		close(client_fd);
	}
}

int communicate(int client_fd) {	
	char op[5];
	char file_name[20];
	char file_full_name[100];
	FILE *file;
	unsigned long long file_size = 0;
	int code;
	char file_format[10];
	char *tmp;
	int i = 0,n;
	char buf[BUF_SIZE];

	char *file_buf;
	char *c_header;
	int c_header_len;
	char *response;
	printf("count: %d\n", count);
	n = read(client_fd, buf, BUF_SIZE);
	printf("read: %d\n", n);
	buf[n] = '\0';
	printf("%s\n", buf);
	sscanf(buf, "%s %s", op, file_name);
	tmp = (char *)malloc(sizeof(char) * 20);
	strcpy(tmp, file_name);

	while(tmp[i] != '.') {
		if (i == strlen(tmp)) {
			break;
		}
		i++;
	}
	
	if (i == strlen(tmp))
		strcpy(file_format, "none");
	else
		strcpy(file_format,tmp + i + 1);
	free(tmp);

	printf("file_type: %s\n", file_format);
	printf("file_name: %s\n", file_name);
	if (!strcmp(file_name, "/")) {
		printf("redirect to index.html\n");
		strcpy(file_name, "/index.html");
	}

	if (!strncmp(op, "GET", 3)) {
		strcpy(file_full_name, DOCUMENT_ROOT);
		strcat(file_full_name, file_name);
		code = check_file(file_full_name);
		printf("code: %d\n", code);
		switch(code) {
			case 200:
				file = fopen(file_full_name, "rb");
				if (file == NULL) {
					fprintf(stderr, "file open error\n");
					return -1;
				}
	
				fseek(file, 0, SEEK_END);
				file_size = ftell(file);
				fseek(file, 0, SEEK_SET);
				file_buf = (char *)malloc(sizeof(char) * (file_size + 1));
				memset(file_buf, 0, file_size);
				fread(file_buf, 1, file_size, file);
				break;
			case 403:
				file_size = strlen(FORBIDDEN) - 2 + strlen(file_name + 1);
				file_buf = (char *)malloc(sizeof(char) * (file_size));
				sprintf(file_buf, FORBIDDEN, file_name);
				break;
			case 404:
				file_size = strlen(NOT_FOUND) - 2 + strlen(file_name);
				file_buf = (char *)malloc(sizeof(char) * (file_size + 1));
				sprintf(file_buf, NOT_FOUND, file_name);
				break;
		}
		c_header_len = header_len(get_status(code), "close", get_mime(file_format));
		c_header = (char *)malloc(sizeof(char) * (c_header_len + 1));
		create_header(c_header, get_status(code), "close", get_mime(file_format));
		response = (char *)malloc(sizeof(char) * (c_header_len + file_size + 1));
		memset(response, 0, c_header_len + file_size);
		strcpy(response, c_header);
		memcpy(response+c_header_len, file_buf, file_size);
		free(c_header);
		free(file_buf);
		printf("write!\n");
		write(client_fd, response, c_header_len + file_size);
		free(response);
		printf("write done\n");
	}
}

int check_file(char *path) {
	int rd = access(path, R_OK);
	int ex = access(path, F_OK);

	if (!ex && !rd)
		return 200;
	else if (!ex && rd)
		return 403;
	else
		return 404;
}

char *get_status(int code) {
	switch(code) {
		case 200:
			return "200 OK";
		case 403:
			return "403 FORBIDDEN";
		case 404:
			return "404 NOT FOUND";
	}
}

char *get_mime(char *format) {
	if (!strcmp(format, "jpg"))
		return "image/jpeg";
	else if (!strcmp(format, "gif"))
		return "image/gif";
	else if (!strcmp(format, "html"))
		return "text/html";
	else if (!strcmp(format, "txt"))
		return "text/plain";
	else if (!strcmp(format, "mp3"))
		return "audio/mpeg";
	else if (!strcmp(format, "mpeg"))
		return "video/mpeg";
	else if (!strcmp(format, "avi"))
		return "video/x-msvideo";
	else if (!strcmp(format, "mp4"))
		return "video/mp4";
	else if (!strcmp(format, "wmv"))
		return "video/x-mv-wmv";
	else
		return "text/html";
}

void print_help(char *path) {
	char *help = "Usage: %s [port]\n";
	printf(help, path);
}

int header_len(char *status, char *connection, char *mime) {
	int imcomplete_header_len = strlen(header);
	int complete_header_len = imcomplete_header_len - (2 + 2 + 2);
	complete_header_len += strlen(status) + strlen(connection) + strlen(mime);

	return complete_header_len;
}

char *create_header(char *c_header, char *status, char *connection, char *mime) {
	sprintf(c_header, header, status, connection, mime);

	return c_header;
}
