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
#define cantCompanieros 7

int main(int argc, char* argv[]){
  pid_t pid = getpid();
  tMessage msg;
	
  key_t llave = ftok(FILE_PATH, 5);
  if (llave < 0) {
   report_and_exit("ftok");
  }
  
  printf("Master[%d] Key is: %x\n", pid, llave);
  int cola = msgget(llave, 0666 | IPC_CREAT);
  if (cola < 0) {
    report_and_exit("msgget");
  }
		
   //Mensaje alguien comprando
   msg.type = TYPE_ALGUIEN_COMPRANDO;
   msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT); //Agregue a la cola un mensaje del tipo alguien comprando
   
   //Mensaje heladera llena
   msg.type = TYPE_HELADERA_LLENA;
   int i;
   for(i = 0; i < cantBotellas; i++){
      msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT); //Agregue a la cola un mensaje del tipo heladera llena
   } 
   
   for(i=0;i < cantCompanieros; i++){
      pid_t p = fork();
      if(p == 0){
	char* arg[]={"./companiero",NULL};
	execvp("./companiero",arg);
      }
    }
  
  for(i = 0;i < cantCompanieros;i++){
    wait(NULL);
  }
   
	return 0;
}



