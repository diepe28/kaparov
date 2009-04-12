#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include <unistd.h>

int potencia (int n)
{
	if (n%2 == 1)
		return 1;
	else return -1;
}

long double arcenesimo (long n)
{
	if (n == 0)
		return 0.0;

    //if ((n % 1000) == 0) sleep(2);
        //printf("arcenesimo de multiplo de 1000: %ld\n", n);

	int divisor = 2*n-1;
	long double respuesta = 1.0;
	respuesta = -1*pow(-1,n)*4/divisor;
	return respuesta;
}

long double enesimo (int n)
{
	int i;
	long double respuesta = 1.0;

	if (n==1)
		return 2;
	if (n==2)
		return 0.33333;

	for (i = 1; i<(2*n)-1;i++)
	{
		if (i%2 == 1)
			respuesta *= i;
		else
			respuesta /= i;
	}
	respuesta /= ((2*n)-1);
	return 2*respuesta;
}

long double sumarN (int n)
{
	int i;
	long double respuesta = 0.00;
	for (i = 1; i<=n; i++)
	{
		printf ("%.30Le\n", respuesta);
		respuesta += arcenesimo(i);
	}
	return respuesta;
}
