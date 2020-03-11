server: build/server.o build/retos.o includes/conf.h
	gcc -o server build/server.o build/retos.o -I./includes/ -lpthread -lcrypto -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu  -lmariadbclient -lpthread -lz -lm -ldl


debug:
	gcc -c src/retos-debug.c -I./includes/ -o build/retos-debug.o -lpthread -lcrypto -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu  -lmariadbclient -lpthread -lz -lm -ldl
	gcc -c src/server-test.c -I./includes/ -o build/server-test.o -lpthread
	gcc -o server-test build/server-test.o build/retos-debug.o -I./includes/ -lpthread -lcrypto -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu  -lmariadbclient -lpthread -lz -lm -ldl


build/server.o: src/server.c includes/conf.h
	gcc -c src/server.c -I./includes/ -o build/server.o -lpthread

build/retos.o: src/retos.c
	gcc -c src/retos.c -I./includes/ -o build/retos.o -lpthread -lcrypto -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu  -lmariadbclient -lpthread -lz -lm -ldl

test:
	./server

clean:
	rm build/*.o
