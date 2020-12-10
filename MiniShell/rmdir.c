#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int j, char* argv[]){
	
	if(argv[2] != NULL){
	  printf("error, se ingresaron mal los parametros\n");
	  return 0;
	}
	
	int res = rmdir(argv[1]);//Si devuelve 0 borra el directorio, en caso de devolver -1 no modifica nada
	
	if(res != 0){
		printf("No se pudo borrar el directorio\n");
	}else{
		printf("Se borro con exito el directorio\n");
	}
	
	return 0;
}
