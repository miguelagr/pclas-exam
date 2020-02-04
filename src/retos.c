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


//inet_ntop(AF_INET, &(ptr)->info.sin_addr, s_ip, INET_ADDRSTRLEN);
//	printf("%016x\n",ptr->info.sin_addr);
//	printf("Conexion desde %s\n", s_ip);
void *rt0(void * all_info)
{

	MYSQL * con;
	MYSQL_RES * res;
	MYSQL_ROW  row;
	con_t *ptr = (con_t*)all_info;
	//printf("Nueva, Conexion\n");
	//printf("fd: %d\n", ptr->fd);
	int counter;
	char s_ip[128];
	char query[256];
	char buffer[1024];
	send(ptr->fd , "Escribe tu nombre: " , strlen("Escribe tu nombre: ") , 0 );
	counter = recv(ptr->fd ,(void *)buffer, 1024,0);
	if(counter == -1)
		counter = 0;
	buffer[counter] = '\0';
	for(counter = 0; buffer[counter++];)
	{
		if(buffer[counter - 1] == '\n')
		{
			buffer[counter - 1] = '\0';
		}
	}
	printf("La longitud es: %d\n", strlen(buffer));
	printf( "Recibi el nombre %s\n", buffer);
	printf("DESDE %016x\n", ptr->info.sin_addr.s_addr);
	printf("DESDE %lu\n", ptr->info.sin_addr.s_addr);
	inet_ntop(AF_INET, &(ptr)->info.sin_addr, s_ip, INET_ADDRSTRLEN);
	printf("%016x\n",ptr->info.sin_addr);
	printf("Conexion desde %s\n", s_ip);
	query[0] = '\0';
	sprintf(query,"SELECT ip FROM becarios WHERE INET_NTOA(ip) = '%s';", s_ip);
	printf("%s",query);
	send(ptr->fd, "OK+0\n",strlen("OK+0\n"),0);
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
			if(mysql_query(con, query))
			{
				printf("No se pudo realizar la consulta\n");
			}
			else
			{
				res = mysql_store_result(con);
				if(res == NULL  && mysql_errno(con))
					printf("Ocurrio un error\n");
				else
				{
					if(res == NULL)
					{

					}
					else
					{
						row = mysql_fetch_row(res);
						if( row == NULL)
						{
							query[0] = '\0';
							sprintf(query,"INSERT INTO becarios(ip, nombre) values (inet_aton('%s'), '%s');", s_ip, buffer);
							printf("%s", query);
							mysql_free_result(res);
							if(mysql_query(con, query))
							{
								printf("No se pudo realizar la consulta\n");
							}
							else
							{
								printf("Se ha actualizado");
							}
							printf("No hay resultados");
						}else
						{
							query[0] = '\0';
							sprintf(query,"UPDATE becarios SET nombre = '%s' WHERE inet_ntoa(ip) = '%s'", buffer, s_ip);
							printf("%s", query);
							mysql_free_result(res);
							if(mysql_query(con, query))
							{
								printf("No se pudo realizar la consulta\n");
							}
							else
							{
								printf("Se ha actualizado");
							}
							printf("No hay resultados");
						}
						//printf("%d", row == NULL);
						//printf("%s\n" , row[0]);
						//printf("\nLa tabla tiene %d columnas\n", mysql_num_fields(res));
						//mysql_free_result(res);
					}
				}
			}
			mysql_close(con);	
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
	char * msg;
	char buffer[1024];
	char reto[128];
	int n1,n2,r;
	int result;
	// Calcula dos numeros enterios aleatorios
	n1 = (random() % 50000) + 1000000;
	n2 = n1 - (random() % 1000);
	if(random()%2)
	{
		sprintf(reto, "%lu + %lu = ", n1, n2);
		send(ptr->fd , reto , strlen(reto) , 0);
		counter = recv(ptr->fd ,(void *)buffer, 1024,0);
		buffer[counter] = '\0';
		sscanf(buffer,"%lu",&result);
		result = result - (n1 + n2);
	}
	else
	{
		sprintf(reto, "%lu - %lu = ", n1, n2);
		send(ptr->fd , reto , strlen(reto) , 0);
		counter = recv(ptr->fd ,(void *)buffer, 1024,0);
		buffer[counter] = '\0';
		sscanf(buffer,"%lu",&result);
		result = result - (n1 - n2);
	}
	
	if(result != 0)
	{
		msg = "NO\n";
		send(ptr->fd, msg, strlen(msg), 0);
	}
	else
	{
		msg = "OK+2\n";
		send(ptr->fd, msg, strlen(msg), 0);
	}
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
		send(ptr->fd, "OK+2\n",3,0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}

void genera_r3( char * ret , int * sol)
{
	int i;
	int j;
	char piv[4];
	char piv_r[4];
	ret[0] = '\0';
	*sol = random() % 10;
	for(j = 0; j < 3; j++)
	{
		piv_r[j] = (random() % ('Z' - 'A')) + 'A';
	}
	piv_r[3] = '\0';
	sprintf(ret, "Encuentra el indice de \"%s\" en la siguiete lista:\n", piv_r);
	for( i = 0; i < 10; i++)
	{
		if(*sol == i)
		{
			for(j = 0; j < 3; j++)
			{
				piv[j] = piv_r[j];
			}
			piv[3] = '\0';
		}else
		{
			for(j = 0; j < 3; j++)
			{
				piv[j] = (random() % ('A' - 'Z')) + 'A';
			}
			piv[3] = '\0';
		}
		if(i == 0)
		{
			strcat(ret, piv);
		}
		else
		{
			strcat(ret, " ");
			strcat(ret, piv);
		}
	}
	strcat(ret, "\nR: ");
	return;
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
	char * msg;
	int stage = 0;
	int sol;
	char str_reto[128];

	do
	{
		genera_r3(str_reto, &sol);
		send(ptr->fd , str_reto , strlen(str_reto) , 0);
		counter = recv(ptr->fd ,(void *)buffer, 1024,0);
		buffer[counter] = '\0';
		if( (buffer[0] - 0x30) == sol)
		{
			stage++;
		}
		else
		{
			msg = "NO\n";
			send(ptr->fd, msg, strlen(msg),0);
			shutdown(ptr->fd,SHUT_RDWR);
			close(ptr->fd);
			free(ptr->self);
			return NULL;
		}
	}while(stage < 3);
	
	msg = "OK+4\n";
	send(ptr->fd, msg, strlen(msg),0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}

int min( int a, int b)
{
	return (a < b? a: b);
}

void genera_reto( char * str , int * solx, int * soly)
{
	int m[3][3];
	int c,d;
	int resx, resy;
	int err;
	int diff;
	int i = random() % 3;
	int j = random() % 3;
	int x = random() % 3;
	int y = random() % 3;
	int diffx;
	int diffy;

	do
	{
		err = 0;
		i = random() % 3;
		j = random() % 3;
		x = random() % 3;
		y = random() % 3;
		diffx = ( i < x? x-i: i-x);
		diffy = ( j < y? y-j: j-y);
		diff = (diffx < diffy? diffy - diffx: diffx - diffy);
		if(((diffx > 1) || (diffy > 1)) && (diff > 0))
			err = 1;
		if((i == x) && (j == y))
			err = 1;
		if( ((diffx == 1) && (diffy == 1)) && ( (x == 1) || (y == 1)))
			err = 1;

	}while ( err );

	for(c = 0; c < 3; c++)
	{
		for(d = 0; d < 3; d++)
		{
			m[c][d] = 0;
		}
	}

	m[i][j] = 1;
	m[x][y] = 1;
	// Calcular el diferencial
	diffx = ( i < x? x-i: i-x);
	diffy = ( j < y? y-j: j-y);

	if((diffx > 1) || (diffy > 1))
	{
		if(diffx == diffy)
		{
			resx = 1;
			resy = 1;
		}

		// Determinamos la linea
		if(i == x)
		{
			resx = i;
			resy = 1;
		}
		// Determinamos la linea
		if(y == j)
		{
			resx = 1;
			resy = j;
		}

		
	}
	else
	{
		// Determinamos la linea
		if(i == x)
		{
			resx = i;
			resy = (min( j , y ) == 0 ? 2:0);
		}else if(y == j)
		{
			resx = (min( i , x ) == 0 ? 2:0);
			resy = j;
		}else
		{
			resy = (min( j , y ) == 0 ? 2:0);
			resx = (min( i , x ) == 0 ? 2:0);
		}
	}

	*solx = resx;
	*soly = resy;
	//printf("punto 1: %d, %d\n", i, j);
	//printf("punto 2: %d, %d\n", x, y);
	//printf("respuesta %d %d\n", resx, resy);
	sprintf(str,"%d %d %d\n%d %d %d\n%d %d %d\n",m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
	return;
}

void *rt4(void * all_info)
{
	// Completa la linea ej (R: 1,2)
	// 0 - -
	// - - -
	// - - 0
	// R: 1,1
	con_t *ptr = (con_t*)all_info;
	int c, i;
	int solx,soly;
	int stage = 0;
	int ansy,ansx;
	char * msg;
	char buffer[1024];
	char str_reto[128];

	msg = "Completa la linea ej (R: 1,2)\n";
	send(ptr->fd , msg , strlen(msg) , 0);
	do
	{
		genera_reto(str_reto,&solx,&soly);
		//printf("%s",str_reto);
		//printf("\n");
		send(ptr->fd , str_reto , strlen(str_reto) , 0);
		msg = "R: ";
		send(ptr->fd , msg , strlen(msg) , 0);
		c = recv(ptr->fd ,(void *)buffer, 1024,0);
		//c = recv(ptr->fd ,(void *)buffer, 1024,0);
		//buffer[c] = '\0';
		//printf("Recibi :%d\n", c);
		ansx = buffer[0] - 0x30;
		ansy = buffer[2] - 0x30;
		if((ansx == solx) && (ansy == soly) && (c != -1))
		{
			stage++;
		}else
		{
			msg = "NO\n";
			send(ptr->fd, msg, strlen(msg),0);
			shutdown(ptr->fd,SHUT_RDWR);
			close(ptr->fd);
			free(ptr->self);
			return NULL;

		}

	}while(stage < 3);
	msg = "OK+2\n";
	send(ptr->fd, msg, strlen(msg),0);
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
}


void *rt5(void * all_info)
{
	// Encuentra el camino que me de la menor suma
	// 9 9 9 9 9 9 9
	// 9 9 9 9 9 9 9
	// 9 9 9 9 9 9 9
	// 9 9 9 9 9 9 9
	// 9 1 1 1 9 9 9
	// 1 1 9 1 1 1 1
	// 9 9 9 9 9 9 9
	// R: 9
	con_t *ptr = (con_t*)all_info;
	int counter;
	int i;
	char buffer[1024];
	char reto[128];
	char d_cmp[128];
	char str_reto[128];
	unsigned char digest[MD5_DIGEST_LENGTH];
	
	shutdown(ptr->fd,SHUT_RDWR);
	close(ptr->fd);
	free(ptr->self);
	return NULL;
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