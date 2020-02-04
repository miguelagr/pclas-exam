// Biblioteca estandar
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <time.h>
// Includes del sistema
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
// External lib
#include <openssl/md5.h>
// Mysql lib
//#include <my_global.h>
#include <mysql.h>
// Includes de usuario
#include "conf.h"

void juego_nombre(int fd,struct sockaddr_in client)
{
	//int fd;
	//struct sockaddr_in client;
	int counter;
	char buffer[1024];
	send(fd , "Escribe tu nombre: " , strlen("Escribe tu nombre: ") , 0 );
	counter = recv( fd ,(void *)buffer, 1024,0);
	send(fd, "OK\n",3,0);
	shutdown(fd,SHUT_RDWR);
	close(fd);
	return;
}

void *rt0(void * all_info)
{

	MYSQL * con;
	MYSQL_RES * res;
	con_t *ptr = (con_t*)all_info;
	char query[128];
	printf("Nueva, Conexion\n");
	printf("fd: %d\n", ptr->fd);
	int counter;
	char buffer[1024];
	send(ptr->fd , "Escribe tu nombre: " , strlen("Escribe tu nombre: ") , 0 );
	counter = recv(ptr->fd ,(void *)buffer, 1024,0);
	buffer[counter] = '\0';
	for(counter = 0; buffer[counter++];)
	{
		if(buffer[counter - 1] == '\n')
		{
			buffer[counter - 1] = '\0';
		}
	}
	printf( "Recibi el nombre %s\n", buffer);
	send(ptr->fd, "OK\n",3,0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	con = mysql_init(NULL);
	if(con == NULL)
	{
		printf("No se pudo inicializar la base de datos\n");
	}
	else
	{
		if ( mysql_real_connect(con, "127.0.0.1","becariosmaster",NULL,"becarios",\
				0,NULL,0) == NULL)
		{
			printf("No se pudo conectar con la base de datos\n");
		}
		else
		{
			if(mysql_query(con, "SELECT * FROM becariostest;"))
			{
				printf("No se pudo realizar la consulta\n");
			}
			else
			{
				res = mysql_store_result(con);
				if(res == NULL  && mysql_errno(con))
					printf("Ocurrio un error\n");
				else
					if(res == NULL)
						printf("No hay resultados");
					else
					{
						printf("La tabla tiene %d columnas\n", mysql_num_fields(res));
					}
			}
				
		}
	}
	return NULL;
}

void *rt1(void * all_info)
{
	// Reto que te da dos numeros y te pide
	// el resultado correcto de la operacion
	con_t *ptr = (con_t*)all_info;
	int counter;
	char buffer[1024];
	char reto[128];
	int n1,n2,r;
	int result;
	// Calcula dos numeros enterios aleatorios
	n1 = (random() % 50000) + 1000000;
	n2 = n1 - (random() % 1000);
	if(random()%2)
	{
		sprintf(reto, "%d + %d = ", n1, n2);
		send(ptr->fd , reto , strlen(reto) , 0);
		counter = recv(ptr->fd ,(void *)buffer, 1024,0);
		buffer[counter] = '\0';
		sscanf(buffer,"%lu",&result);
		result = result - (n1 + n2);
		printf("%u\n",result);
	}
	else
	{
		sprintf(reto, "%d - %d = ", n1, n2);
		send(ptr->fd , reto , strlen(reto) , 0);
		counter = recv(ptr->fd ,(void *)buffer, 1024,0);
		buffer[counter] = '\0';
		sscanf(buffer,"%lu",&result);
		result = result - (n1 - n2);
		printf("%u\n",result);
	}
	if(result)
		send(ptr->fd, "NO\n",3,0);
	else
		send(ptr->fd, "OK\n",3,0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}

void *rt2(void * all_info)
{
	// Reto que te pide calcular el md5
	con_t *ptr = (con_t*)all_info;
	int counter;
	int i;
	char buffer[1024];
	char reto[128];
	char d_cmp[128];
	char str_reto[128];
	unsigned char digest[MD5_DIGEST_LENGTH];
	// Calcula dos numeros enterios aleatorios
	for(i = 0; i < (random() % 10 ) + 20; i++)
		reto[i] = 'A' + (random() % ('Z' - 'A'));
	reto[i] = '\0';
	sprintf(str_reto, "MD5(%s) = ", reto);
	send(ptr->fd , str_reto , strlen(str_reto) , 0);
	counter = recv(ptr->fd ,(void *)buffer, 1024,0);
	buffer[counter] = '\0';
	for(i = 0; i < counter; i++)
		if(isalpha(buffer[i]) || isdigit(buffer[i]))
			continue;
		else
			buffer[i] = '\0';
	MD5(reto, strlen(reto), digest);
	for(i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&d_cmp[i*2], "%02x" , digest[i]);
	printf("%s\n",d_cmp);
	if(strcmp(d_cmp, buffer))
		send(ptr->fd, "NO\n",3,0);
	else
		send(ptr->fd, "OK\n",3,0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}

void *rt3(void * all_info)
{
	// Encuentra el indice de "baz" en la siguiente lista:
	// foo nob dgp bazz soup no
	// R: 
	con_t *ptr = (con_t*)all_info;
	int counter;
	int i;
	char buffer[1024];
	char reto[128];
	char d_cmp[128];
	char s_ip[32];
	char str_reto[128];
	unsigned char digest[MD5_DIGEST_LENGTH];
	// Calcula dos numeros enterios aleatorios
	inet_ntop(AF_INET, &(ptr)->info.sin_addr, s_ip, INET_ADDRSTRLEN);
	printf("%016x\n",ptr->info.sin_addr);
	printf("Conexion desde %s\n", s_ip);
	for(i = 0; i < (random() % 10 ) + 20; i++)
		reto[i] = 'A' + (random() % ('Z' - 'A'));
	reto[i] = '\0';
	sprintf(str_reto, "MD5(%s) = ", reto);
	send(ptr->fd , str_reto , strlen(str_reto) , 0);
	counter = recv(ptr->fd ,(void *)buffer, 1024,0);
	buffer[counter] = '\0';
	for(i = 0; i < counter; i++)
		if(isalpha(buffer[i]) || isdigit(buffer[i]))
			continue;
		else
			buffer[i] = '\0';
	MD5(reto, strlen(reto), digest);
	for(i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&d_cmp[i*2], "%02x" , digest[i]);
	printf("%s\n",d_cmp);
	if(strcmp(d_cmp, buffer))
		send(ptr->fd, "NO\n",3,0);
	else
		send(ptr->fd, "OK\n",3,0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}

void *rt4(void * all_info)
{
	// Completa la linea ej (R: 1,2)
	// 0 - -
	// - - -
	// - - 0
	// R: 1,1
	con_t *ptr = (con_t*)all_info;
	int counter;
	int i;
	char buffer[1024];
	char reto[128];
	char d_cmp[128];
	char str_reto[128];
	unsigned char digest[MD5_DIGEST_LENGTH];
	// Calcula dos numeros enterios aleatorios
	for(i = 0; i < (random() % 10 ) + 20; i++)
		reto[i] = 'A' + (random() % ('Z' - 'A'));
	reto[i] = '\0';
	sprintf(str_reto, "MD5(%s) = ", reto);
	send(ptr->fd , str_reto , strlen(str_reto) , 0);
	counter = recv(ptr->fd ,(void *)buffer, 1024,0);
	buffer[counter] = '\0';
	for(i = 0; i < counter; i++)
		if(isalpha(buffer[i]) || isdigit(buffer[i]))
			continue;
		else
			buffer[i] = '\0';
	MD5(reto, strlen(reto), digest);
	for(i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&d_cmp[i*2], "%02x" , digest[i]);
	printf("%s\n",d_cmp);
	if(strcmp(d_cmp, buffer))
		send(ptr->fd, "NO\n",3,0);
	else
		send(ptr->fd, "OK\n",3,0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}