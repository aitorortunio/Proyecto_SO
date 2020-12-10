#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define cantModos 3

int main(int j, char* argv[]){
	
	char modos[cantModos];
	char* ruta = argv[1];
	
	strcpy(modos, argv[2]);
	
	int numModo = strtol(modos, 0, 8);//Convierte el string a numero en base octal
	
	if(argv[3] != NULL){
	  printf("error, se ingresaron mal los parametros\n");
	  return 0;
	}
	
	int modificacion = chmod(ruta, numModo);//Cambia los permisos si tuvo exito devuelve 0, en caso contrario devuelve -1
	
	if(modificacion == -1){
		printf("error, no se pudieron modificar los permisos\n");
	}else{
		printf("Permisos modificados correctamente\n");
	}
	
	return 0;
}
