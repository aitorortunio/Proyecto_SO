#include <stdlib.h>
#include <stdio.h>

int main(int j, char* argv[]){
	
	if(argv[2] != NULL){
	  printf("error, se ingresaron mal los parametros\n");
	  return 0;
	}
	
	FILE* archivo = fopen(argv[1], "r");//"r" lo abro para lectura
	char caracter;
	
	if(archivo == NULL){
		printf("No se pudo abrir el archivo\n");
		fflush(NULL);
	}else{
		caracter = fgetc(archivo); // obtengo el siguiente caracter
		while(caracter != EOF){
			putchar(caracter);
			caracter = fgetc(archivo);
		}
		
		fclose(archivo);
		printf("\n");
	}
	
	return 0;
}
