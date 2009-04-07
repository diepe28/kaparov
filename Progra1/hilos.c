#include <stdio.h>
//#include <time.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>
//#include <sys/time.h>

#include "hilos.h"
#include "Taylor.h"
#include "Variables.h"

static jmp_buf scheduler_env;
static jmp_buf * thread_env;

void timer_signal (int sig)
{
  //keep_going = 0;
    signal (SIGALRM, timer_signal);
	//printf("Estoy en el timer\n");
	longjmp(scheduler_env, 1);

}


unsigned int establecer_timer (unsigned int usegundos)
{
  /*struct itimerval old, new;
  new.it_interval.tv_usec = 0;
  new.it_interval.tv_sec = 0;
  new.it_value.tv_usec = (long int) usegundos;
  new.it_value.tv_sec = 0;
  if (setitimer (ITIMER_REAL, &new, &old) < 0)
    return 0;
  else
    return old.it_value.tv_sec;*/
	//return alarm(segundos);
}

void imprimirRespuestas();

void ejecutar_scheduler(void) {

    int val;
	int count;
	int numActual;
	
	val = 0;
	count = 0;
	numActual = 0;

	//if (modoActual == Expropiativo)
	//	establecer_timer(QUANTUM);	
	
    val = setjmp(scheduler_env);
	
	while (count < NUM_HILOS) {
		count++;
		if (modoActual == Expropiativo)
			ejecutar_hilo_ex(count-1);
		else
			ejecutar_hilo(count-1);
		
	}
    
	if (!val) {
		
        //ejecutar_hilo();
    } else {
		
		
		
		// Revisar si ya los  hilos terminaron
		imprimirRespuestas();
		printf ("VALOR acum = %d\n", CANT_TIQUETES_ACUM[NUM_HILOS-1]);
		if (CANT_TIQUETES_ACUM[NUM_HILOS-1] == 0) 
		  return;
		
		numActual = numSgte();
		
		printf("Principal: Transferir control al hilo %d\n", numActual);
		
		if (modoActual == Expropiativo) {
		  int old = establecer_timer(QUANTUM);
		  printf("Viejo timer: %d\n", old);
		}
		
		longjmp(thread_env[numActual], numActual+1);
				
    }

}

void thread_init ()
{
	srand(time(NULL));
	thread_env = malloc (sizeof(jmp_buf)*NUM_HILOS);

	if (modoActual == Expropiativo)  {
		signal (SIGALRM, timer_signal);
		//sys_timer_create (CLOCK_REALTIME, 0, 0);		 
	}
	
}

void thread_destroy ()
{
	free (thread_env);
}


void ejecutar_hilo(int n) {
	
	int num;
	
    if (!(num = setjmp(thread_env[n]))) {
		printf("Hilo: Ingreso al hilo %d\n", num);
	} else {
		num--;
		//getchar();
	}
	       int tiquetes = CANT_TIQUETES[num];
	       int trabajo = CANT_TRABAJO[num];
	
		int j = 0;
		
			while (j < QUANTUM * tiquetes)
			{
			  j++;
			  //ITERACION_ACTUAL[num]++;
			  RESPUESTAS[num] += arcenesimo(ITERACION_ACTUAL[num]++);
			}			
		
		printf("Hilo %d: Salvado estado para la iteracion %d\n", num, ITERACION_ACTUAL[num]);
	
	if (trabajo <= ITERACION_ACTUAL[num])
		CANT_TIQUETES [num] = 0;
	

	longjmp(scheduler_env, ITERACION_ACTUAL[num]);

	
}

void ejecutar_hilo_ex(int n) {

	int num;
	
    if (!(num = setjmp(thread_env[n]))) {
		printf("Hilo: Ingreso al hilo ex %d\n", num);
	} else {
		num--;
		//getchar();
	}
	       
	int tiquetes = CANT_TIQUETES[num];
	int trabajo = CANT_TRABAJO[num];
	
		
			while (ITERACION_ACTUAL[num] < trabajo)
			{
			  //ITERACION_ACTUAL[num]++;
			  RESPUESTAS[num] += arcenesimo(ITERACION_ACTUAL[num]++);
			}			
		
		printf("Hilo %d: Salvado estado para la iteracion ex %d\n", num, ITERACION_ACTUAL[num]);
	
	if (trabajo <= ITERACION_ACTUAL[num])
	{
		CANT_TIQUETES [num] = 0;
		longjmp(scheduler_env, ITERACION_ACTUAL[num]);
	}
	

}


int numSgte ()
{
	int iter;
	printf ("Lottery!\n");
	for (iter = 0; iter<NUM_HILOS; iter++)
	{
		if (iter == 0)
			CANT_TIQUETES_ACUM[iter] = CANT_TIQUETES[iter];
		else
			CANT_TIQUETES_ACUM[iter] = CANT_TIQUETES[iter-1]+ CANT_TIQUETES[iter];
	}
	printf ("Middle Lottery!\n");
	int al = aleatorio (CANT_TIQUETES_ACUM[NUM_HILOS-1]);
	printf ("Alea Lottery!\n");
	int i = 0;
	
	while (CANT_TIQUETES_ACUM[i] < al)
	{
		printf ("Al: %d y acum = %d\n",al, CANT_TIQUETES_ACUM[i]);
		i++;
	}
	printf ("Sgte hilo: %d\n", i);
	return i;	

}

int aleatorio (int max)
{
	if (max == 0) return 0;
	return rand()%max;
}

void imprimirRespuestas ()
{
	int i;
	printf ("Estado de respuestas:\n");
	for (i = 0;i<NUM_HILOS;i++)
	{
		printf ("Hilo %d lleva %.30lle \n", i, RESPUESTAS[i]);
	}
		printf ("!\n");
}

