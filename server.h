#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define true 1
#define BUF_SIZE 1024

void init(unsigned int port);		// 이니셜라이징 함수
int communicate(int client_fd);	// 클라이언트와 통신하는 함수
int check_file(char *path);			// 파일의 존재 유무와 접근 권한을 확인하는 함수
char *get_status(int code);			// check_file이 주는 아웃풋에 따른 헤더에 들어갈 스테이터스를 반환하는 함수
char *get_mime(char *format);		// 보내는 파일의 확장자에 따른 mime을 리턴하는 함수
void print_help(char *path);		// 도움말 페이지를 출력하는 함수
int header_len(char *status, char *connection, char *mime);	// 헤더의 길이를 계산하는 함수
char *create_header(char *header, char *status, char *connection, char *mime);	// 헤더를 완성하는 함수