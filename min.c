#include <stdio.h>
int a[5][7] = {
	{9,9,9,9,9,9,9},
	{9,9,9,0,0,0,0},
	{9,1,1,1,9,9,9},
	{1,1,9,1,1,1,1},
	{9,9,9,9,9,9,9}
};

int min(int a , int b)
{
	return (a < b ? a:b);
}

int path(int posy, int posx, int dir , int init)
{
	int P[7];
	int x;
	if(init)
	{
		for(x = 0; x < 5; x++)
		{
			P[x] = path(x,0,0,0);
			printf("P[%d] = %d\n" , x, P[x]);

		}
		printf("calculando el minimo");
		return min(P[0],min(P[1],min(P[2],min(P[3],P[4]))));
	}

	else
	{
		switch (dir)
		{
			// Cuando viene de atras
			case 0:
				if(posx == 6)
					return a[posy][posx];
				if(posy == 0)
					return a[posy][posx] + min(path(posy+1, posx,1,0),path(posy, posx+1,0,0));
				if(posy == 4)
					return a[posy][posx] + min(path(posy-1, posx,2,0),path(posy, posx+1,0,0));
				return a[posy][posx] + min(path(posy + 1, posx, 1,0),min(path(posy,posx+1,0,0),path(posy-1,posx,2,0)));
				break;
			// Cuando viene de arriba
			case 1:
				if(posy == 0)
					return a[posy][posx] + path(posy, posx+1,0,0);
				if(posy == 4)
					return a[posy][posx] + path(posy, posx+1,0,0);
				return a[posy][posx] +  min(path(posy,posx+1,0,0),path(posy+1,posx,1,0));
				break;

			// Cuando viene de abajo
			case 2:
				if(posy == 0)
					return a[posy][posx] + path(posy, posx+1,0,0);
				if(posy == 4)
					return a[posy][posx] + path(posy, posx+1,0,0);
				return a[posy][posx]  + min(path(posy,posx + 1,0,0),path(posy - 1,posx,2,0));
		}
	}
}


int main( void )
{
	printf("El minimo es : %d\n", path(0,0,0,1));
	return 0;
}
