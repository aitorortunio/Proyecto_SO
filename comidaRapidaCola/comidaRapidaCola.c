#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <pthread.h>
#include "./shared.h"
#define cantClientes 50
#define cantCocineros 3
#define cantMesas 30

int cola;
tMessage msg;

void *limpiador(void* i){
	/*
		Espero a que haya una mesa sucia
		Limpio la mesa sucia
		Hay una mesa mas libre 
	*/
	while(1){
		sleep(1);
		msgrcv(cola, &msg, SIZE_MSG, TYPE_MESA_SUCIA,0);
		printf("El limpiador limpio una mesa, hay una mesa mas libre \n");
		msg.type = TYPE_MESA_LIBRE;
		msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
	}	
}

void *camarero(void* i){
	/*
		  Espero a que me envien algun mensaje del tipo Pedido menu
			Si el mensaje tiene el tipo de menu como menu de carne
				Espero a que haya un plato de carne en la cola de comidas
				Retiro el plato de carne de la cola de comidas
				Agrego un mensaje indicando que hay un lugar mas para plato de carne en la cola de comidas
				Entrego el plato de carne al cliente
				Sino
					Si el mensaje tiene el tipo de menu como menu vegetariano
						Espero a que haya un plato vegetariano en la cola de comidas
						Retiro el plato vegetariano de la cola de comidas
						Agrego un mensaje indicando que hay un lugar mas para plato vegetariano en la cola de comidas
						Entrego el plato vegetariano al cliente
	*/	
	while(1){
		sleep(1);
		msgrcv(cola, &msg, SIZE_MSG, TYPE_PEDIDO_MENU,0); 
		if(msg.tipoMenu == MENU_DE_CARNE){//El menu es de carne
			msgrcv(cola, &msg, SIZE_MSG, TYPE_CANT_CARNE_LLENO,0);//Espero a que haya un plato de carne en la cola de comidas
			msg.type = TYPE_CANT_CARNE_VACIA;
			msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
			printf("Soy el camarero retire un plato de carne\n");		
			sleep(1);
			msg.type = TYPE_ENTREGADO_CARNE;
			msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
			printf("Soy el camarero hice la entrega de un pedido de carne\n");
		}else{//El menu es vegetariano
			msgrcv(cola, &msg, SIZE_MSG, TYPE_CANT_VEG_LLENO,0);//Espero a que haya un plato vegetariano en la cola de comidas
			msg.type = TYPE_CANT_VEG_VACIA;
			msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
			printf("Soy el camarero retire un plato vegetariano\n");
			sleep(1);
			msg.type = TYPE_ENTREGADO_VEG;
			msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);		
			printf("Soy el camarero hice la entrega de un pedido vegetariano\n");
		}
	}
}		

void *cliente(void* i){
	/*
		Estoy esperando a recibir un mensaje indicando que hay una mesa libre
			Si el tipo de pedido es de carne
				Agrego a la cola un mensaje del tipo pedido menu y al mensaje le asigno el tipoMenu en menu de carne
				Espero a que me traigan el plato de carne
				Como la comida que me trajeron
				Agrego a la cola un mensaje de tipo mesa sucia
			Sino(si el pedido es vegetariano)
				Agrego a la cola un mensaje del tipo pedido menu y al mensaje le asigno el tipoMenu en menu vegetariano
				Espero a que me traigan el plato vegetariano
				Como la comida que me trajeron
				Agrego a la cola un mensaje de tipo mesa sucia
	 */
	 int id = (int)i;
	 int tipoPedido;//1 = carne , 2 = veg
	 while(1){
		sleep(1);
		tipoPedido = rand() % 2;
			msgrcv(cola, &msg, SIZE_MSG, TYPE_MESA_LIBRE,0);
			if(tipoPedido == 1){//El pedido es de carne
				msg.type = TYPE_PEDIDO_MENU;
				msg.tipoMenu = MENU_DE_CARNE;
				msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
				
				printf("Soy el cliente %d ya hice el pedido de carne\n", id);
				msgrcv(cola, &msg, SIZE_MSG, TYPE_ENTREGADO_CARNE,0);
				printf("Soy el cliente %d estoy comiendo \n", id);
				sleep(2);
				printf("Soy el cliente %d ya termine de comer, me voy del restaurante \n", id);
				msg.type = TYPE_MESA_SUCIA;
				msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
				
			}else{//El pedido es vegetariano
				msg.type = TYPE_PEDIDO_MENU;
				msg.tipoMenu = MENU_VEGETARIANO;
				msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
					
				printf("Soy el cliente %d ya hice el pedido vegetariano\n", id);
				
				msgrcv(cola, &msg, SIZE_MSG, TYPE_ENTREGADO_VEG,0);
				printf("Soy el cliente %d estoy comiendo \n", id);
				sleep(2);
					
				printf("Soy el cliente %d ya termine de comer, me voy del restaurante \n", id);
				msg.type = TYPE_MESA_SUCIA;
				msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);				
			}
	 }
}

void *cocinero(void* i){
	/* 
		Estoy esperando a recibir un mensaje indicando que hay un lugar en la cola de comidas
			Verifico si el mensaje es que hay un lugar para meter un plato de carne
				Agrego a la cola un plato de carne
			Sino
				Agrego a la cola un plato vegetariano
	 */
	 int id = (int)i;
	 while(1){
		 sleep(1);
			 msgrcv(cola, &msg, SIZE_MSG,-2,0); //Recibo un mensaje
			 if(msg.type == TYPE_CANT_CARNE_VACIA){ 
					msg.type = TYPE_CANT_CARNE_LLENO;
					msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);//Agrego a la cola un plato de carne
					printf("Soy el cocinero %d agregue un plato de carne a la cola \n", id); 
			 }else{//Cocinamos un plato vegetariano
				 if(msg.type == TYPE_CANT_VEG_VACIA){
					msg.type = TYPE_CANT_VEG_LLENO;
					msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);//Agrego a la cola un plato vegetariano
					printf("Soy el cocinero %d agregue un plato vegetariano a la cola \n", id); 
				 }
			 }
	}
}

int main(int argc, char* argv[]){
   //Creo los hilos
   pthread_t clientes[cantClientes], cocineros[cantCocineros], camareros, limpiadores;
   pid_t pid = getpid();
	
   //Creo la llave	
   key_t llave = ftok(FILE_PATH, 5);
   if (llave < 0) {
	report_and_exit("ftok");
   }
  
  //Creo la cola
  printf("Master[%d] Key is: %x\n", pid, llave);
  cola = msgget(llave, 0666 | IPC_CREAT);
  if (cola < 0) {
    report_and_exit("msgget");
  }
  
 int i;
 for(i = 0; i < cantMesas; i++){ //Agrega a la cola los mensajes de mesas libres
	msg.type = TYPE_MESA_LIBRE;
	msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
 }
  
  for(i= 0; i < 5; i++){//Agrega a la cola los mensajes de cantidad de platos de carne vacios
	 msg.type = TYPE_CANT_CARNE_VACIA;
	msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
  } 
  
  for(i = 0; i < 5; i++){//Agrega a la cola los mensajes de cantidad de platos vegetarianos vacios
	msg.type = TYPE_CANT_VEG_VACIA;
	msgsnd(cola,&msg,SIZE_MSG,IPC_NOWAIT);
  } 
  
	for(i=0; i<cantClientes;i++){//Hilos para los clientes
		pthread_create(&clientes[i], NULL, cliente, (void*)i);
	}
	
	for(i=0; i<cantCocineros;i++){//Hilos para los cocineros
		pthread_create(&cocineros[i], NULL, cocinero, (void*)i);
	}
	
	pthread_create(&camareros, NULL, camarero, (void*) i);//Hilo para el camarero
	pthread_create(&limpiadores, NULL, limpiador, (void*) i);//Hilo para el limpiador	
		
	//Joins	
	for(i=0;i<cantClientes;i++){
		pthread_join(clientes[i], NULL); //Join para cliente
	}	
	
	for(i=0;i<cantCocineros;i++){
		pthread_join(cocineros[i], NULL); //Join para cocineros
	}
	
	pthread_join(camareros, NULL); //Join para camarero
	pthread_join(limpiadores, NULL); //Join para limpiadores
	
  return 0;
  
}


