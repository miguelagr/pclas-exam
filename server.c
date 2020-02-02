#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 9052



void juego_nombre(int fd,struct sockaddr_in client)
{
	int counter;
	char buffer[1024];
	send(fd , "Escribe tu nombre:" , strlen("Escribe tu nombre:") , 0 );
	counter = recv( fd ,(void *)buffer, 1024,0);
	send(fd, "OK\n",3,0);
	shutdown(fd,SHUT_RDWR);
	close(fd);
	return;
}

int main(int argc, char const **argv)
{ 
	int fd_s, fd_c;
	int counter;
	struct sockaddr_in addr;
	int opt = 1;
	int addrlen = sizeof(addr);
	char buffer[1024] = {0};
	char *hello = "Hello from server\n";

	if ((fd_s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		printf("No se ha podido crear el socker"), exit(1);

	if (setsockopt(fd_s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		printf("No se pudo poner socket a la escucha"), exit(1);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( PORT );

	if (bind(fd_s, (struct sockaddr *)&addr,  sizeof(addr))<0) 
		printf("Fallo al hacer bind"), exit(1);

	if(listen(fd_s, 1000) < 0)
		printf("Fallo al poner en escucha"), exit(1);

	while(1)
	{
		if ((fd_c = accept(fd_s, (struct sockaddr *)&addr,(socklen_t*)&addrlen))<0)
			printf("No se pudo aceptar conexion"), exit(1);

		//printf("Conexion de %s\n",inet_ntoa(addr.sin_addr));

		if(fork())
			continue;
		else
			break;

	}
	juego_nombre(fd_c, addr);
	return 0;
} 

