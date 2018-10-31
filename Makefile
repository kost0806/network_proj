server.o :
	gcc -c -o server.o server.c

main.o :
	gcc -c -o main.o main.c

server :
	gcc -o server server.o main.o

clean :
	rm *.o server
