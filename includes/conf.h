#include <netinet/in.h>
#define PORT 7790


typedef struct{
	int fd;
	struct sockaddr_in info;
} con_t;

