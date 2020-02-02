#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
	int fd;
	char chal[128];
	struct sockaddr_in addr;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if( fd == -1)
	{
		printf("No se pudo crear el socket");
		return 0;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7790);
	if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <=0 )
		printf("Direccion invalida\n");
	if( connect( fd  , (struct sockaddr *) &addr, sizeof(addr)) < 0)
			printf("No se pudo conectar");
	recv(fd,chal,128,0);

	int i = 0;
	int j = 0;
	char n1[32];
	char n2[32];
	printf("%s\n",chal);
	while(1)
	{
		if(chal[i] <= 0x39 && chal[i] >= 0x30)
			break;
		else
			i++;
	}
	while(1)
	{
		if(chal[i] <= 0x39 && chal[i] >= 0x30)
		{
			n1[j] = chal[i];
			j++;
			i++;
		}
			
		else
		{
			n1[j] = '\0';
			break;
		}
	}

	for(i = 0; i< 10; i++)
	{
		printf("%c",n1[i]);
	}
	printf("\n");

	printf("n1 = %d", atoi(n1));




	send(fd,"1010",strlen("1010"),0);
//	write(fd, "1010",5);
	return 0;
}
