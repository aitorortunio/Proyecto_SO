#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int j, char* argv[]){
	
	if(argv[2] != NULL){
	  printf("error, se ingresaron mal los parametros\n");
	  return 0;
	}
	
	if (argv[1] == NULL){
		printf("Ingrese tambien el nombre del comando que desea ver \n");
		return 0;
	}
	
	char * comando = argv[1];
	
	if(strcmp(comando, "open") == 0){
		printf("Crea un nuevo archivo en la ruta donde se encuentra la minishell,\nen caso de error no se produce ninguna modificacion\n");
	}else{
		if(strcmp(comando, "mkdir") == 0){
			printf("Crea un nuevo directorio en la ruta donde se encuentra la minishell,\nen caso de error no se produce ninguna modificacion\n");
		}else{
			if(strcmp(comando, "ls") == 0){
				printf("Lista todos los archivos y carpetas en el directorio en la ruta especificada,\nen caso de error no se produce ninguna modificacion\n");
			}else{
				if(strcmp(comando, "rmdir") == 0){
					printf("Elimina un directorio en la ruta donde se encuentra en la minishell,\nen caso de error no se produce ninguna modificacion\n");
				}else{
					if(strcmp(comando, "fopen") == 0){
						printf("Muestra el contenido de un archivo en la ruta donde se encuentra la minishell,\nen caso de error no se produce ninguna modificacion\n");
					}else{
						if(strcmp(comando, "man") == 0){
							printf("El manual muestra una ayuda y detalles sobre un comando especifico.\n");
							printf("Aqui puede verse un listado de todos los comandos disponibles.\n");
							printf("open--> Abrir archivo\nfopen--> Mostrar contenido de archivo\n");
							printf("mkdir--> Crear un nuevo directorio\nrmdir--> Eliminar un directorio\n");
							printf("ls--> listar contenido de un directorio\nchmod--> Modificar permisos\n");
						}else{
								if(strcmp(comando, "chmod") == 0){
									printf("Se modifican los permisos del archivo\nEj: 777: Cualquier usuario tiene acceso total\n");
								}
						}
					}
				}
				
			}
			
			
		}
		
		
		
	}
}
