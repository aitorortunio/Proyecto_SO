#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#define tamanioBuffer 25
#define SIZE_COMMAND sizeof(char)*300

int main(){
	
	int cod_error;
	pid_t pid;
	
	char * args[tamanioBuffer];
	int i;
	for(i = 0; i <  tamanioBuffer; i++){
		args[i] = NULL;	
	}
	
	char * comando = (char*) malloc(SIZE_COMMAND);
	
	
	
	char * palabra;
	char ruta[tamanioBuffer];
	 
	while(1){
		printf("Comando--> ");
		
		fgets(comando, SIZE_COMMAND, stdin); //Guarda en comando lo que ingreso por teclado
			
		int longitud = strlen(comando); //Necesito agregar caracter de fin de linea al comando
		comando[--longitud] = '\0';
		
		
		int pos = 0;
		palabra = strtok(comando, " ");
		while(palabra != NULL){
			args[pos] = palabra;
			palabra = strtok(NULL, " ");
			pos++;
		}
		args[pos] = NULL;
		
		//Lo primero es verificar si no hay que cortar la ejecucion
		//si el comando es "exit", se finaliza la ejecucion
		//args[0] es la primer palabra del comando, que es la instruccion que se quiere ejecutar		
		if(strcmp(args[0], "exit") == 0){
			free(comando);
			exit(0);
		}
		
		strcpy(ruta, "./");
		strcat(ruta, args[0]);
		args[0] = ruta;
		
		pid = fork();
		if(pid == -1){
			printf("error al hacer fork\n");
		}else{
			if(pid == 0){//Estamos en el hijo
				cod_error = execv(args[0], args);
				if(cod_error == -1 ){
					perror("error");
					fflush(NULL);
				}
			}else{//Estamos en el padre
				wait(NULL); // El padre espera a que termine su hijo
			}
			
		}
		
	}
	
	return 0;
	
}

