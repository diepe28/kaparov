#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <signal.h>
#include <time.h>

#include "hilos.h"
#include "Taylor.h"
#include "Variables.h"
#include "interfaz.h"

#define TIPO_TIMER CLOCK_REALTIME
#define SENAL_TIMER SIGRTMIN

static timer_t scheduler_timer;
static int timerEstablecido;
static int contadorHilos;

static jmp_buf scheduler_env;
static jmp_buf * thread_env;

void timer_handler(int sig, siginfo_t *si, void *uc);
void establecer_timer_handler();
void inicializar_timer();
void finalizar_timer();
void establecer_timer(int segundos);

void ejecutar_hilo_ex(int n);
void ejecutar_hilo(int n);
void ejecutar_scheduler();

void imprimirRespuestas();
int numSgte ();
int aleatorio (int max);

void inicializar_timer() {

    struct sigevent sev;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SENAL_TIMER;
    sev.sigev_value.sival_ptr = &scheduler_timer;

    timer_create(TIPO_TIMER, &sev, &scheduler_timer);
}

void finalizar_timer() {
    timer_delete(scheduler_timer);
}

void establecer_timer(int segundos) {

    struct itimerspec its;

    its.it_value.tv_sec = segundos;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = segundos;
    its.it_interval.tv_nsec = 0;

    timer_settime(scheduler_timer, 0, &its, NULL);

    timerEstablecido = 1;

    printf("Timer establecido con quantum: %d\n", segundos);
}

void timer_handler(int sig, siginfo_t *si, void *uc) {

	//printf("Estoy en el timer\n");
	longjmp(scheduler_env, 1);



}

void establecer_timer_handler() {

    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    sa.sa_handler = 0;
    sa.sa_sigaction = timer_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SENAL_TIMER, &sa, NULL);
}

void ejecutar_scheduler() {

    int val;
	//int count;
	int numActual;

	val = 0;
	contadorHilos = 0;
	numActual = 0;

        while(1) {

    val = setjmp(scheduler_env);

    // Inicializar los hilos
	while (contadorHilos < NUM_HILOS) {
		contadorHilos++;
		if (contadorHilos > 1) printf("Hilo %d inicializado\n", (val-1));
		if (modoActual == Expropiativo)
			ejecutar_hilo_ex(contadorHilos-1);
		else
			ejecutar_hilo(contadorHilos-1);
	}
	if (!timerEstablecido)
        printf("Hilo %d inicializado\n", (val-1));

	if (val) {

        // Establecer timer
        if (modoActual == Expropiativo && !timerEstablecido) {
            establecer_timer(QUANTUM);
        }

		// Revisar si ya los  hilos terminaron
		imprimirRespuestas();
		printf("VALOR acum = %d\n", CANT_TIQUETES_ACUM[NUM_HILOS-1]);

        /*while (1) {
            arcenesimo(val);
        }*/

		// Revisar si ya los  hilos terminaron
		if (CANT_TIQUETES_ACUM[NUM_HILOS-1] == 0)
		  return;

		numActual = numSgte();

		if (numActual >= NUM_HILOS) return;

		printf("Principal: Transferir control al hilo %d\n", numActual);

		longjmp(thread_env[numActual], numActual+1);

    }

        } // while

}

void thread_init ()
{
	srand(time(NULL));
	thread_env = malloc (sizeof(jmp_buf)*NUM_HILOS);

	if (modoActual == Expropiativo)  {
	    establecer_timer_handler();
	    inicializar_timer();
	    timerEstablecido = 0;
	}

}

void thread_destroy ()
{
    if (modoActual == Expropiativo) {
        finalizar_timer();
        timerEstablecido = 0;
    }
	free (thread_env);
}

void ejecutar_hilo(int n) {

	int num, j;
    double progreso;

    if (!(num = setjmp(thread_env[n]))) {
		printf("Hilo: Ingreso al hilo %d\n", num);
		longjmp(scheduler_env, (n + 1));
	} else {
		num--;
	}

    int tiquetes = CANT_TIQUETES[num];
    int trabajo = CANT_TRABAJO[num];

	j = 0;

    while (j < QUANTUM * tiquetes)
    {
        j++;
        RESPUESTAS[num] += arcenesimo(ITERACION_ACTUAL[num]++);
    }

	if (trabajo <= ITERACION_ACTUAL[num])
		CANT_TIQUETES [num] = 0;

    printf("Hilo %d: Salvado estado para la iteracion %ld\n", num, ITERACION_ACTUAL[num]);
    progreso = (double)ITERACION_ACTUAL[num] / (double)CANT_TRABAJO[num];
    if (progreso > 1.0) progreso = 1.0;
    printf("Hilo %d: Progreso %f\n", num, progreso);
    actualizarBarra(num, progreso);


	longjmp(scheduler_env, ITERACION_ACTUAL[num]);

}

void ejecutar_hilo_ex(int n) {

	int num;
	double progreso;

    if (!(num = setjmp(thread_env[n]))) {
		printf("Hilo %d: Ingreso modo ex. Tiquetes = %d, Trabajo = %ld\n",
            n, CANT_TIQUETES[n], CANT_TRABAJO[n]);
        longjmp(scheduler_env, (n + 1));
	} else {
		num--;
	}

	int trabajo = CANT_TRABAJO[num];

    printf("Hilo %d: Iniciar iteracion ex %ld\n", num, ITERACION_ACTUAL[num]);

    while (ITERACION_ACTUAL[num] < trabajo)
    {
        RESPUESTAS[num] += arcenesimo(ITERACION_ACTUAL[num]++);
    }

    printf("Hilo %d: Iteracion ex %ld finalizada\n", num, ITERACION_ACTUAL[num]);

    progreso = (double)ITERACION_ACTUAL[num] / (double)CANT_TRABAJO[num];
    if (progreso > 1.0) progreso = 1.0;
    printf("Hilo %d: Progreso %f\n", num, progreso);
    actualizarBarra(num, progreso);

	if (trabajo <= ITERACION_ACTUAL[num])
	{
		CANT_TIQUETES [num] = 0;
        printf("Hilo %d finalizado\n", num);
	}

	longjmp(scheduler_env, 1);

}

int numSgte ()
{
	int iter;
	for (iter = 0; iter<NUM_HILOS; iter++)
	{
		if (iter == 0)
			CANT_TIQUETES_ACUM[iter] = CANT_TIQUETES[iter];
		else
			CANT_TIQUETES_ACUM[iter] = CANT_TIQUETES[iter-1] + CANT_TIQUETES[iter];
	}

	int al = aleatorio (CANT_TIQUETES_ACUM[NUM_HILOS-1]);
	printf ("al: %d\n", al);
	int i = 0;

	while (i < NUM_HILOS && (CANT_TIQUETES_ACUM[i] < al || CANT_TIQUETES[i] == 0))
	{
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
		printf ("Hilo %d lleva %.30Le \n", i, RESPUESTAS[i]);
	}
    printf ("!\n");
}

