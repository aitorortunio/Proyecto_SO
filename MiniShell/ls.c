#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>

int main(int j, char* argv[]){
	
	if(argv[2] != NULL){
	  printf("error, se ingresaron mal los parametros\n");
	  return 0;
	}
	
	char * directorioAListar = argv[1];
	struct dirent * directorio;
	DIR * dir = opendir(directorioAListar);//Devuelve nulo cuando llega alfinal del directorio
	
	if(dir == NULL){
		printf("No se pudo listar el directorio\n");
		fflush(NULL);
	}else{	
		directorio = readdir(dir);
		while(directorio != NULL){
			printf("%s \n", directorio->d_name);
			directorio = readdir(dir);
		}		
		closedir(dir);
	}
	
	return 0;	
}
