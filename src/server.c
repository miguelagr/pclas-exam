// Standar lib
#include <stdlib.h> 
#include <stdio.h> 
#include <pthread.h>
#include <time.h>
// system includes
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
// User includes
#include "conf.h"
#include "retos.h"

int main(int argc, char const **argv)
{ 
	int i;
	void * ptr;
	int fd_s, fd_c;
	struct sockaddr_in addr;
	int opt = 1;
	int addrlen = sizeof(addr);
	void * (*ret_arr[7])(void *);
	struct timeval tv;

	ret_arr[0] = rt0;
	ret_arr[1] = rt1;
	ret_arr[2] = rt2;
	ret_arr[3] = rt3;
	ret_arr[4] = rt4;
	ret_arr[5] = rt5;

	for(i = 0; i < 7; i++)
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

	srand(time(NULL));

	while(1)
	{
		if ((fd_c = accept(fd_s, (struct sockaddr *)&addr,(socklen_t*)&addrlen))<0)
			printf("No se pudo aceptar conexion"), exit(1);

		
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		setsockopt(fd_c, SOL_SOCKET, SO_RCVTIMEO , &tv, sizeof(tv));
		ptr = malloc(sizeof(con_t));
		((con_t * ) ptr)->self = ptr;
		((con_t * ) ptr)->fd = fd_c;
		((con_t * ) ptr)->info = addr;
		pthread_create( &((con_t * ) ptr)->id , NULL, ret_arr[i], ptr);
		//juego_nombre(fd_c, addr);
		//free(ptr);
	}
	return 0;
} 
