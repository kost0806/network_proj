run :
	gcc -o server server.c

server.o :
	gcc -c server.c

clear :
	rm server.o server.c
