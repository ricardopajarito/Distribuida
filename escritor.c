#include <minix/mthread.h>
#include <minix/semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*Variables Globales*/
int rc=0;

//base de datos
FILE * fp;

/*Semaforos*/
sem_t mutex,db;

/*Id de proceso*/
int pid;

/*Estructura*/
typedef struct datos{
	char nombre[20];
	char apellido[20];
}Datos;

void escribirenbasedatos(){

 	extern FILE *fp;
 	char caracter;
 	int cont=0;
 	char cadena[] = "Mostrando el uso de fwrite en un fichero.\n";
 	
 	fp = fopen ( "fichero.txt", "r+" );
 	
 	while((caracter = fgetc(fp)) != EOF){
 			if (caracter=='\n')
 			{
 				fwrite( cadena, sizeof(char), sizeof(cadena), fp ); //char cadena[]... cada posición es de tamaño 'char'
 				cont=1; 
 			}
	}
 	if (cont==0)
 	{
 		fwrite( cadena, sizeof(char), sizeof(cadena), fp ); //char cadena[]... cada posición es de tamaño 'char'
 	}
 	fclose ( fp );	

}

void *escritor(void *p){

		extern sem_t mutex,db;
		int *id_es;
        id_es=(int *)p;

		/*Sección Residual*/

		sem_wait(&db); //true si entra y logra escribir, falso sigue esperando
		printf("\nEscritor %d Escribiendo...\n",*id_es);
		escribirenbasedatos();
		sem_post(&db);
		mthread_exit(NULL);

}


int main(int argc, char const *argv[])
{
	pid=fork();
	/*Inicialización de Semáforos*/
	sem_init(&mutex,0,1);
	sem_init(&db,0,1);

	mthread_thread_t hilo1;
	mthread_create(&hilo1,NULL,escritor,(void *)&pid);
	mthread_join(hilo1,NULL);

	sem_destroy(&mutex);
	sem_destroy(&db);
	return 0;
	return 0;
}