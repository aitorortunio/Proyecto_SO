#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#define cantClientes 50
#define cantCocineros 3

sem_t s_mesas_libres, s_mesas_sucias, s_cola_comida_vacia,s_cola_comida_llena, s_pedido_hecho, s_pedido_entregado;
pthread_t clientes[cantClientes], cocineros[cantCocineros], camareros, limpiadores;

void *limpiador(void* i){
	while(1){
		sem_wait(&s_mesas_sucias);
		printf("El limpiador limpio una mesa, hay una mesa mas libre \n");
		sem_post(&s_mesas_libres);
	}
}
	
void *camarero(void* i){	
		/*
		  Verifico si me hicieron algun pedido
			Verifico si hay una comida en la cola de comidas
				Saco una comida de la cola
				Entregar al cliente
		*/
			while(1){
				sem_wait(&s_pedido_hecho);//Verifico si me hicieron algun pedido
				sem_wait(&s_cola_comida_llena);//Verifico si hay comida en la cola
				printf("Soy el camarero retire un plato de la cola de comidas\n");
				sem_post(&s_cola_comida_vacia);
				printf("Soy el camarero ya entrege el plato de comida\n");
				sem_post(&s_pedido_entregado);
				
			}
}

void *cliente(void* i){
	/*
	 Ver si hay una mesa disponible 
			Hacer un pedido al camerero
			Cuando me traen la comida, como y luego me voy de la mesa
	 */
	
	int id = (int)i;
	while(1){
			sem_wait(&s_mesas_libres); // Verifico si hay una mesa libre
			sem_post(&s_pedido_hecho); //Hago el pedido
			printf("Soy el cliente %d ya hice el pedido \n", id);
			sleep(1);
			//ahora tengo que esperar a que me traigan la comida
			sem_wait(&s_pedido_entregado);//Verifico si el pedido fue entregado
				printf("Soy el cliente %d estoy comiendo \n", id);
				sleep(2);
				printf("Soy el cliente %d ya termine de comer, me voy del restaurante \n", id);
				sem_post(&s_mesas_sucias);
		}		
}


void *cocinero(void* i){
	/* 
	  Verificar que hay un espacio en la cola de comidas
		Creo una nueva comida y lo agrego a la cola
	 */
	int id = (int)i;
	while(1){
		sem_wait(&s_cola_comida_vacia);//Verifico si hay un espacio en la cola de comidas
		printf("Soy el cocinero %d agregue una comida a la cola \n", id);
		sleep(3);
		sem_post(&s_cola_comida_llena); //Agrego una nueva comida a la cola
	}		
}


int main (){
	//Inicializo los semaforos
	sem_init(&s_cola_comida_llena, 0,0);
	sem_init(&s_cola_comida_vacia, 0,10);	
	sem_init(&s_mesas_libres,0,30);
	sem_init(&s_mesas_sucias,0,0);
	sem_init(&s_pedido_hecho, 0,0);
	sem_init(&s_pedido_entregado,0,0);
	
	int i;
	
	for(i=0; i<cantClientes;i++){//Hilos para los clientes
		pthread_create(&clientes[i], NULL, cliente, (void*)i);
	}
	
	for(i=0; i<cantCocineros;i++){//Hilos para los cocineros
		pthread_create(&cocineros[i], NULL, cocinero, (void*)i);
	}
		
		pthread_create(&limpiadores, NULL, limpiador, (void*) i);//Hilo para el limpiador
		pthread_create(&camareros, NULL, camarero, (void*) i);//Hilo para el camarero
		
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

