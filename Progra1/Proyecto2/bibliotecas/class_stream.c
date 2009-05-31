#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* read_line(FILE *p);
int FINarchivo;

/*Lee una linea del archivo*/
char* read_line(FILE *p){
	char *linea=(char*)malloc(sizeof(char)*101);
	int iT=0,i=0;
	char caracter;
	char* buftmp;
	FINarchivo=1;
	while((caracter=getc(p)) != EOF){
		FINarchivo=0;
		if(i==100){
			buftmp=(char*)malloc(sizeof(char)*(strlen(linea)+101));
			strcpy(buftmp,linea);
			free(linea);
			linea=buftmp;
			i=0;
		}
		if(caracter=='\n'){
			linea[iT]='\0';
			break;
		}
		else{
			linea[iT]=caracter;
		}
		i++;
		iT++;
	}
	//caso de llegar al final del archivo
	if(caracter==EOF){
		FINarchivo=1;
		linea[iT]='\0';
	}
	return linea;
}
