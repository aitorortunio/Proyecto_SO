#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <semaphore.h>
#define cantCompanieros 2
#define cantBotellas 14

/* ALGORITMO BOTELLASLECHE
	Me quedo con la exclusividad de la heladera
	Verifico si no hay leche en la heladera
		Libero la heladera
		Verifico si alguien fue a comprar
			Espero a que repongan las leches de la heladera
			Consumi una leche de la heladera
		Sino
			Voy a comprar leches en el supermercado
			Ya volvi de comprar leches
			Repongo las leches en la heladera
			Aviso que repuse las heladeras
	Sino
		Consumi una leche de la heladera
		Libero la heladera 
*/
 
struct numCompaniero{
	int idCompaniero;
};

typedef struct numCompaniero num;

//Declaro las variables de los semaforos
sem_t s_comprando, s_heladera_llena, s_heladera_cerrada;

void * companiero(void* args){
	num* arg= (num*) args;
	int numero = arg->idCompaniero;
	int j;
	
	while(1){
		sleep(1);
		sem_wait(&s_heladera_cerrada);//Me quedo con la exclusividad de la heladera
		if(sem_trywait(&s_heladera_llena) != 0){//Verifico si no hay leche en la heladera
			sem_post(&s_heladera_cerrada);//Libero la heladera
			if(sem_trywait(&s_comprando) != 0){//Verifico si alguien fue a comprar
				printf("Soy el companiero %d espero a que repongan las leches de la heladera\n",numero);
				sem_wait(&s_heladera_llena);//Espero a que me avisen que repusieron las botellas
				printf("Soy el companiero %d consumi una leche de la headera\n",numero);
			}else{
				printf("Soy el companiero %d voy a comprar las leches\n",numero);
				sleep(1);
				printf("Soy el companiero %d ya volvi del supermercado\n",numero);
				for(j = 0; j < cantBotellas; j++){//Repongo las botellas de leche
					sem_post(&s_heladera_llena);
				}
				sem_post(&s_comprando);//Aviso que ya repuse las botellas
			}
			
		}else{
			printf("Soy el companiero %d consumi una leche de la heladera\n",numero);
			sem_post(&s_heladera_cerrada);//Libero la heladera	
		}
		
	}
}







int main(){
	
	pthread_t companieros[cantCompanieros];

	//Inicializacion de los semaforos
	sem_init(&s_comprando, 0,1);
	sem_init(&s_heladera_llena, 0,cantBotellas);
	sem_init(&s_heladera_cerrada, 0,1);
	
	//Fork
	int i;
	for(i = 0; i < cantCompanieros;i++){
		num* comp = (num*) malloc(sizeof(num));
		comp->idCompaniero = i+1;
		pthread_create(&companieros[i], NULL, companiero, (void*) comp);
	}
	
	//Join
	for(i = 0; i < cantCompanieros; i++){
		pthread_join(companieros[i], NULL);
	}
	
	return 0;	
}






