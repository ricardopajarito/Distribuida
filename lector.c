#include <minix/mthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Variables Globales*/
int rc=0;

//base de datos
FILE * baseDatos;

/*Semaforos*/
sem_t mutex,db;

/*Id de proceso*/
int pid;

/*Estructura*/
typedef struct datos{
	char nombre[20];
	char apellido[20];
}Datos;

void leerenbasedatos(){
	      
	extern FILE *archivo;
	char caracter;
	
	archivo = fopen("fichero.txt","r");
	
	if (archivo == NULL){
            printf("\nError de apertura del archivo. \n\n");
        }else{
            printf("\nEl contenido del archivo de prueba es \n\n");
            while((caracter = fgetc(archivo)) != EOF){
				printf("%c",caracter);
	    	}
        }
    fclose(archivo);

}
void *leer(void *args){
		extern sem_t mutex,db;
		extern int rc;
		int *id_le;
		id_le=(int *)args;
		
		sem_wait(&mutex); //espera para entrar a la seccion critica
		rc++;
		if(rc == 1)
		   sem_wait(&db); //damos preferencia a los lectores

		
		printf("\nLector %d Leyendo...\n",*id_le);
		leerenbasedatos(); 
		sem_post(&mutex);//otros lectores pueden entrar a la seccion crítica
		sem_wait(&mutex); //el lector se quiere ir
		rc--;

		if(rc == 0)//verifica que no este en la seccion critica
		  sem_post(&db);//el escritor puede entrar

		sem_post(&mutex);
		mthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	pid=fork();
	/*Inicialización de Semáforos*/
	sem_init(&mutex,0,1);
	sem_init(&db,0,1);

	mthread_thread_t hilo1;
	mthread_create(&hilo1,NULL,leer,(void *)&pid);
	mthread_join(hilo1,NULL);

	sem_destroy(&mutex);
	sem_destroy(&db);
	return 0;
}