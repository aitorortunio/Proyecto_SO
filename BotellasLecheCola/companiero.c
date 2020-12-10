#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include "./shared.h"
#define cantBotellas 6

/* Algoritmo botellas leche con cola
  
    Verifico que no haya leche en la heladera 
      Verifico si alguien fue a comprar
	Espero a que repongan una de las leches
	Consumo una leche de la heladera
      Sino
	Voy a comprar las botellas de leche al supermercado
	Repongo las leches 
	Aviso que ya volvi de comprar
    Sino
      Consumo una leche de la heladera
  
  */
  
 int main(int argc, char* arg[]){
  pid_t pid = getpid();
  tMessage msg;
	
  //Creo la llave
  key_t llave = ftok(FILE_PATH, 5);
  if (llave < 0) {
    report_and_exit("ftok");
  }
  //Creo la cola
 // printf("Master[%d] Key is: %x\n", pid, llave);
  int cola = msgget(llave, 0666 | IPC_CREAT);
  if (cola < 0) {
    report_and_exit("msgget");
  }
  
  int j;
  while(1){
    sleep(1);
    //Verifico que no haya leche en la heladera
    if(msgrcv(cola, &msg, SIZE_MSG, TYPE_HELADERA_LLENA, IPC_NOWAIT) ==-1){
	//Verifico si alguien fue a comprar
	if(msgrcv(cola, &msg, SIZE_MSG, TYPE_ALGUIEN_COMPRANDO, IPC_NOWAIT) == -1){
	  printf("Soy el companiero %d espero a que repongan las leches de la heladera\n", pid);
	  msgrcv(cola, &msg, SIZE_MSG, TYPE_HELADERA_LLENA, 0);//Espero a que repongan una de las leches
	  printf("Soy el companiero %d consumi una leche de la headera\n", pid);
	}else{
	  printf("Soy el companiero %d voy a comprar las leches\n", pid);
	  sleep(2);//Simula el tiempo que demora un companiero en comprar las leches
	  printf("Soy el companiero %d ya volvi del supermercado\n", pid);
	  msg.type = TYPE_HELADERA_LLENA;//Asigno el tipo de mensaje
	  for(j = 0; j < cantBotellas ; j++){//Repongo las botellas de leche en la heladera
	    msgsnd(cola, &msg, SIZE_MSG, IPC_NOWAIT);
	  }

	  sleep(1);
	  msg.type = TYPE_ALGUIEN_COMPRANDO;//Asigno el tipo del mensaje 
	  msgsnd(cola, &msg, SIZE_MSG, IPC_NOWAIT);//Pongo el mensaje en la cola para avisar que ya volvi de comprar
	}
    }else{
	printf("Soy el companiero %d consumi una leche de la heladera\n", pid);
	sleep(1);
    }
  }
    return 0;
}
