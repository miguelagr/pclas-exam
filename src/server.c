#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 

#include "conf.h"
#include "retos.h"

int main(int argc, char const **argv)
{ 
	int i;
	int fd_s, fd_c;
	struct sockaddr_in addr;
	int opt = 1;
	int addrlen = sizeof(addr);

	for(i = 0; i < 5; i++)
		if(! (opt = fork())) break;

	if(opt)
		while(1);

	opt = 1;

	if ((fd_s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("No se ha podido crear el socket"), exit(1);

	if (setsockopt(fd_s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		printf("No se pudo poner socket a la escucha"), exit(1);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( PORT + i);

	if (bind(fd_s, (struct sockaddr *)&addr,  sizeof(addr))<0) 
		printf("Fallo al hacer bind"), exit(1);

	if(listen(fd_s, 1000) < 0)
		printf("Fallo al poner en escucha"), exit(1);

	while(1)
	{
		if ((fd_c = accept(fd_s, (struct sockaddr *)&addr,(socklen_t*)&addrlen))<0)
			printf("No se pudo aceptar conexion"), exit(1);

		if(fork())
			continue;
		else
			break;

	}
	juego_nombre(fd_c, addr);
	return 0;
} 
