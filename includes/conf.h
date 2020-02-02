#include <netinet/in.h>
#include <pthread.h>
#define PORT 7790


typedef struct{
	int fd;
	struct sockaddr_in info;
	void * self;
	pthread_t id;
} con_t;

