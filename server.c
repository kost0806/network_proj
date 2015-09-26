#include "server.h"

int main(int argc, char **argv, char **envp) {
	if (argc != 2) {
		fprintf(stderr, "argument error!", argv[0]);
		print_help(argv[0]);
		exit(2); // arg error
	}

	init(atol(argv[1]));
}
