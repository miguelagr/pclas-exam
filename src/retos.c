#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>

void juego_nombre(int fd,struct sockaddr_in client)
{
	int counter;
	char buffer[1024];
	send(fd , "Escribe tu nombre: " , strlen("Escribe tu nombre: ") , 0 );
	counter = recv( fd ,(void *)buffer, 1024,0);
	send(fd, "OK\n",3,0);
	shutdown(fd,SHUT_RDWR);
	close(fd);
	return;
}
