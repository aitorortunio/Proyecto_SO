#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <string.h> 

int main(int j, char* argv[]){
	
	if(argv[2] != NULL){
		printf("error, se ingresaron mal los parametros\n");
		return 0;
	}
	
	int res = mkdir(argv[1], 0777);//Si se completo con exito mkdir() devolvera 0, caso contrario no se creara el directorio y devolvera -1
	
	if(res != 0){//mkdir() devolvio 0
		printf("No se pudo crear el directorio\n");
	}else{//mkdir() devolvio -1
		printf("Se creo correctamente el directorio\n");
		
	}
	
	return 0;
}
