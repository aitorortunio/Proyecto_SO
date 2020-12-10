#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int j, char* argv[]){
	
	if(argv[2] != NULL){
		printf("error, se ingresaron mal los parametros\n");
		return 0;
	}
	
	char buffer[30];
	int tamanioInput; 
	
	
	int res = open(argv[1], O_CREAT);//Devuelve un entero no negativo si fue exitoso, 
									//en caso contrario se devolvera -1 y no se modificara ni creara ningun archivo
	
	if(res >= 0){
		tamanioInput = read(res, buffer, 30);
		while(tamanioInput > 0){
			write(1, &buffer, tamanioInput);
			tamanioInput = read(res, buffer, 30);
		}
		printf("Se creo el archivo correctamente\n");
	}else{
		printf("No se pudo crear el archivo\n");
	}
	

    close(res);
	return 0;
}
