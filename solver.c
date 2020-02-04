#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int i = 0;
	int j = 0;
	int fd;
    char op ;
	char chal[1024];
    int c;
    char n1[32];
	char n2[32];
    char resp[128];
	struct sockaddr_in addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if( fd == -1)
	{
		printf("No se pudo crear el socket");
		return 0;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7791);
	if(inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <=0 )
		printf("Direccion invalida\n");
	if( connect( fd  , (struct sockaddr *) &addr, sizeof(addr)) < 0)
			printf("No se pudo conectar");
	c = recv(fd,chal,1024,0);
    chal[c] = '\0';
	printf("%s\n",chal);
    i = 0;
	while(1)
	{
		if(chal[i] > 0x39 || chal[i] < 0x30)
			i++;
		else
			break;
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

    op  = chal[i+1];
    while(1)
	{
		if(chal[i] > 0x39 || chal[i] < 0x30)
			i++;
		else
			break;
	}
    j = 0;
	while(1)
	{
		if(chal[i] <= 0x39 && chal[i] >= 0x30)
		{
			n2[j] = chal[i];
			j++;
			i++;
		}
		else
        {
			n1[j] = '\0';
			break;
        }
	}


    printf("%s\n", n1);
    printf("%s\n", n2);
	printf("n1 = %d\n", atoi(n1));
    printf("n1 = %d\n", atoi(n2));

    if(op == '+')
    {
        sprintf(resp, "%lu", atoi(n1)+ atoi(n2));
        printf("%s\n",resp);
        send(fd,resp,strlen(resp),0);
    }
    else
    {
        sprintf(resp, "%lu", atoi(n1) - atoi(n2));
        printf("%s\n",resp);
        send(fd,resp,strlen(resp),0);

    }
	    
    c = recv(fd,chal,1024,0);
    chal[c] = '\0';
	printf("\n%s\n",chal);
	return 0;
}
