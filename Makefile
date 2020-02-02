server: build/server.o build/retos.o includes/conf.h
	gcc -o server build/server.o build/retos.o -I./includes/ -lpthread -lcrypto

build/server.o: src/server.c includes/conf.h
	gcc -c src/server.c -I./includes/ -o build/server.o -lpthread

build/retos.o: src/retos.c
	gcc -c src/retos.c -I./includes/ -o build/retos.o -lpthread -lcrypto

test:
	./server

clean:
	rm build/*.o
