#include <string.h>

#define MASCARA 7
void criptografia_Encriptar(char * cadena);
void criptografia_Desencriptar(char * cadena);
/*==================================================
Provee Criptografia simple sobre un string
===================================================*/
void criptografia_Encriptar(char * cadena)
{
    int largo =0;
    largo = strlen(cadena);
    int i = 0;
    while (i<largo){
        cadena[i]= cadena[i]+MASCARA;
        i++;
    }
}


void criptografia_Desencriptar(char* cadena)
{
    int largo = 0;
    largo = strlen(cadena);
    int i = 0;
    while (i<largo){
        cadena[i]=cadena[i]-MASCARA;
        i++;
    }
}


/*main()
{
	char cadena[] = "Hola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo BonitoHola Mundo Bonito";
	int largo = 0;
	printf("%s\n",cadena);
	criptografia_Encriptar(cadena);
	printf("%s\n",cadena);
	criptografia_Desencriptar(cadena);
	printf("%s\n",cadena);
	
}*/
