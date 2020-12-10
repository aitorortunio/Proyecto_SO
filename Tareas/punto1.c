#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h> 
#include <ctype.h>

char finish;
#define SIZE_FINISH sizeof(char)

struct tareaA{
	int tiempo;
	int color;// 0 = Azul, 1 = rojo
	int tipo; //0 parcial - 1 total
};

typedef struct tareaA msgTareaA;

#define SIZE_MSGA sizeof(msgTareaA)

struct tareaB{
	int tipo; //0 verificacion - 1 reparacion
	int tiempo;
	int repeticion;
};

typedef struct tareaB msgTareaB;

#define SIZE_MSGB sizeof(msgTareaB)

struct tareaC{
	int tiempo;
	int tipo; //0 reparacion de llanta - 1 rotacion y balanceo
	int cantRuedas;
};

typedef struct tareaC msgTareaC;

#define SIZE_MSGC sizeof(msgTareaC)

int pipeA[2],pipeB[2],pipeC[2], pipeCoordinador[2];
sem_t s_A, s_B, s_C, t_A, t_B, t_C;

void inicializarMsjA(void* msgA){	
	int tipoTarea = rand() % 2;
	int color = rand() % 2;
	struct tareaA* msg = (struct tareaA*) msgA;
	
	if(color == 0){//azul
		msg->color = 0;
	}else{//rojo
		msg->color = 1;
	}

	if(tipoTarea == 0){//parcial
		msg->tipo = 0;
		msg->tiempo = 1;

	}else{//total
		msg->tipo = 1;
		msg->tiempo = 1;
	}

}


void inicializarMsjB(void* msgB){
	int tipoTarea = rand() % 2;
	struct tareaB* msg = (struct tareaB*) msgB;

	if(tipoTarea == 0){//verificacion
		msg->tipo = 0;
		msg->tiempo = 1;

	}else{//reparacion
		msg->tipo = 1;
		msg->tiempo = 2;
	}

}


void inicializarMsjC(void* msgC){
	int tipoTarea = rand() % 2;
	int ruedas = rand() % 4;
	struct tareaC* msg = (struct tareaC*) msgC;

	if(tipoTarea == 0){//reparacion de llanta
		if(ruedas == 0){//Hay que reparar 1 rueda
			msg->tipo = 0;
			msg->cantRuedas = ruedas;
			msg->tiempo = 1;

		}else{
			if(ruedas == 1){//Hay que reparar 2 ruedas
				msg->tipo = 0;
				msg->cantRuedas = ruedas;
				msg->tiempo = 2;
			}else{
				if(ruedas == 2){//Hay que reparar 3 ruedas
					msg->tipo = 0;
					msg->cantRuedas = ruedas;
					msg->tiempo = 3;
				}else{//Hay que reparar 4 ruedas
					msg->tipo = 0;
					msg->cantRuedas = ruedas;
					msg->tiempo = 4;
				}
			}
		}
	}else{//rotacion y balanceo
		msg->tipo = 1;
		msg->tiempo = 3;
	}
}

void ejecutarCuatro(){
	printf("Se ejecutan 4 tareas \n");
	msgTareaA msgA;
	inicializarMsjA(&msgA);
	write(pipeA[1], &msgA, SIZE_MSGA);

	msgTareaB msgB;
	inicializarMsjB(&msgB);
	msgB.repeticion = 2;
	write(pipeB[1], &msgB, SIZE_MSGB);	
}

void ejecutarCinco(){
	printf("Se ejecutan 5 tareas \n");

	msgTareaA msgA;
	inicializarMsjA(&msgA);
	write(pipeA[1], &msgA, SIZE_MSGA);

	msgTareaB msgB;
	inicializarMsjB(&msgB);
	msgB.repeticion = 1;
	write(pipeB[1], &msgB, SIZE_MSGB);

	msgTareaC msgC;
	inicializarMsjC(&msgC);
	write(pipeC[1], &msgC, SIZE_MSGC);
}

void ejecutarSeis(){
	printf("Se ejecutan 6 tareas \n");

	msgTareaA msgA;
	inicializarMsjA(&msgA);
	write(pipeA[1], &msgA, SIZE_MSGA);

	msgTareaB msgB;
	inicializarMsjB(&msgB);
	msgB.repeticion = 2;
	write(pipeB[1], &msgB, SIZE_MSGB);

	msgTareaC msgC;
	inicializarMsjC(&msgC);
	write(pipeC[1], &msgC, SIZE_MSGC);
}


void* hiloA(void* msgA){
	while(1){
		sem_wait(&s_A);
		printf("{A} Estoy haciendo trabajos de pintura sobre el vehiculo \n");
		 struct tareaA* msg = (struct tareaA*) msgA;

		if(msg->color == 0){//azul
			printf("{A} Estoy pintando el auto de color azul \n");
		}else{//rojo
			printf("{A} Estoy pintando el auto de color rojo \n");
		}

		if(msg->tipo == 0){//parcial
			printf("{A} Estoy trabajando en tipo parcial \n");
			sleep(1);
		}else{//total
			printf("{A} Estoy trabajando en tipo total\n");
			sleep(3);
		}
		sem_post(&t_A);
	}
}


void* hiloB(void* msgB){
	while(1){
		sem_wait(&s_B);
		printf("{B} Estoy haciendo trabajos de frenos sobre el vehiculo \n");
		 struct tareaB* msg = (struct tareaB*) msgB;

		if(msg->tipo == 0){//verificacion
			printf("{B} Estoy trabajando en tipo verificacion \n");
			sleep(1);
		}else{//reparacion
			printf("{B} Estoy trabajando en tipo reparacion\n");
			sleep(2);
		}
		sem_post(&t_B);
	}
}

void* hiloC(void* msgC){
	while(1){
		sem_wait(&s_C);
		printf("{C} Estoy haciendo trabajos sobre las ruedas del vehiculo \n");
		struct tareaC* msg = (struct tareaC*) msgC;

		if(msg->tipo == 0){//reparacion de llanta
			if(msg->cantRuedas == 1){ //una sola rueda
				printf("{C}  Estoy trabajando en tipo reparacion de llanta (1 rueda)\n");
				sleep(1);
			}else{
				if(msg->cantRuedas == 2){ //dos ruedas
					printf("{C} Estoy trabajando en tipo reparacion de llanta (2 ruedas)\n");
					sleep(2);
				}else{
					if(msg->cantRuedas == 3){ //tres ruedas
						printf("{C} Estoy trabajando en tipo reparacion de llanta (3 ruedas)\n");
						sleep(3);
					}else{//cuatro rueadas
						printf("{C} Estoy trabajando en tipo reparacion de llanta (4 ruedas)\n");
						sleep(4);
					}
				}
			}
		}else{//rotacion y balanceo
			printf("{C} Estoy trabajando en tipo rotacion y balanceo\n");
			sleep(3);
		}
		sem_post(&t_C);
	}
}


void tareaA(){
	pthread_t hilo1, hilo2;
	msgTareaA msgA;

	//Creamos los hilos
	pthread_create(&hilo1, NULL, hiloA, (void*) &msgA);
	pthread_create(&hilo2, NULL, hiloA, (void*) &msgA);
	
	//Cerramos los pipes
	close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeCoordinador[0]);

	while(1){
		read(pipeA[0], &msgA, SIZE_MSGA);
		
		sem_post(&s_A);
		sem_post(&s_A);

		sem_wait(&t_A);
		sem_wait(&t_A);
		
		write(pipeCoordinador[1], &finish, SIZE_FINISH);
	}
	
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	
}

void tareaB(){
	pthread_t hilo1, hilo2;
	msgTareaB msgB;

	//Creamos los hilos
	pthread_create(&hilo1, NULL, hiloB, (void*) &msgB);
	pthread_create(&hilo2, NULL, hiloB, (void*) &msgB);

	//Cerramos los pipes
	close(pipeA[0]);
	close(pipeA[1]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeCoordinador[0]);
	
	while(1){
		read(pipeB[0], &msgB, SIZE_MSGB);

		//Sabemos que en todos los ciclos almenos una vez se tiene que ejecutar la tarea B, 
		//si la repeticion es 2 se tiene que ejecutar una vez mas
		
		sem_post(&s_B);
		if(msgB.repeticion == 2){
			sem_post(&s_B);
		}

		sem_wait(&t_B);
		if(msgB.repeticion == 2){
			sem_wait(&t_B);
		}
		
		write(pipeCoordinador[1], &finish, SIZE_FINISH);
	}
	
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
}


void tareaC(){	
	pthread_t hilo1, hilo2;
	msgTareaC msgC;

	//Creamos los hilos
	pthread_create(&hilo1, NULL, hiloC, (void*) &msgC);
	pthread_create(&hilo2, NULL, hiloC, (void*) &msgC);

	//Cerramos los pipes
	close(pipeA[0]);
	close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[1]);
	close(pipeCoordinador[0]);

	while(1){
		read(pipeC[0], &msgC, SIZE_MSGC);
		
		sem_post(&s_C);
		sem_post(&s_C);

		sem_wait(&t_C);
		sem_wait(&t_C);
		
		write(pipeCoordinador[1], &finish, SIZE_FINISH);
	}
	
		pthread_join(hilo1, NULL);
		pthread_join(hilo2, NULL);
}

void coordinador(){	
	char aux;
	int cantTareas;
	
	//Cerramos los pipes
	close(pipeA[0]);
	close(pipeB[0]);
	close(pipeC[0]);
	close(pipeCoordinador[1]);

	while(1){
		cantTareas = (rand() % 3) + 4;
		if(cantTareas == 4){//4 tareas
			printf("Entre en tarea 4\n");
			ejecutarCuatro();
		}else{//5 tareas
			if(cantTareas == 5){
				printf("Entre en tarea 5\n");
				ejecutarCinco();
			}else{//6 tareas
				printf("Entre en tarea 6\n");
				ejecutarSeis();
			}
		}

		if(cantTareas == 4){
			read(pipeCoordinador[0], &aux, SIZE_FINISH);
			read(pipeCoordinador[0], &aux, SIZE_FINISH);
		}else{
			read(pipeCoordinador[0], &aux, SIZE_FINISH);
			read(pipeCoordinador[0], &aux, SIZE_FINISH);
			read(pipeCoordinador[0], &aux, SIZE_FINISH);
		}
		
		printf("Finalizo un ciclo ejecuto de nuevo\n");
		printf("\n");
		printf("\n");
		}
}

int main(){

	pid_t pidA,pidB,pidC;

	//Creamos el pipeA
	if (pipe(pipeA) == -1) {
		perror("PipeA");
		exit(EXIT_FAILURE);
	}
  //Creamos el pipeB
	if (pipe(pipeB) == -1) {
		perror("PipeB");
		exit(EXIT_FAILURE);
	}
	//Creamos el pipeC
	if (pipe(pipeC) == -1) {
		perror("PipeC");
		exit(EXIT_FAILURE);
	}
	//Creamos el pipeCoordinador
	if (pipe(pipeCoordinador) == -1) {
		perror("PipeCoordinador");
		exit(EXIT_FAILURE);
	}
	
	pidA = fork();
	if(pidA == 0){
		sem_init(&s_A, 0, 0);
		sem_init(&t_A, 0, 0);
		tareaA();
	}else{
		pidB = fork();
		if(pidB == 0){
			sem_init(&s_B, 0, 0);
			sem_init(&t_B, 0, 0);
			tareaB();
		}else{
			pidC = fork();
			if(pidC == 0){
				sem_init(&s_C, 0, 0);
				sem_init(&t_C, 0, 0);
				tareaC();
			}else{
				coordinador();
			}
		}
	}

	return 0;
}
