#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int pid;

int envia(int puerto, char msj[20], char ip[20]){
	int sock;		/*Socket Emitido*/
	struct sockaddr_in echoServerAddr;    /*Estrutura para el server*/
	struct sockaddr_in fromaddr;
	unsigned short echoServerPort = puerto;
	char * serverIP = ip;
	char * echoString = msj;
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sock == -1){
		printf("Error sock...\n");
		return -1;
	}
	echoServerAddr.sin_family = AF_INET;
	echoServerAddr.sin_addr.s_addr = inet_addr(serverIP);
	echoServerAddr.sin_port = htons(echoServerPort);
	bzero( &(echoServerAddr.sin_zero), 8);
	sendto(sock, msj, strlen(msj), 0, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr));
}

int recibe(int puerto, int puerto2, char msj[20]){
	pid = fork();
	if(pid==0){
		while(1){
			int sock;
			struct sockaddr_in echoServerAddr;  
			struct sockaddr_in echoClientAddr;
			char Buffer []="";
			unsigned short echoServerPort = puerto;
			int reciveMessageSize;
			sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if(sock == -1){
				printf("Error sock\n");
				exit(-1);	
			} 
			struct timeval timeout = {2, 0};
			setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
			echoServerAddr.sin_family = AF_INET;
			echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			echoServerAddr.sin_port = htons(echoServerPort);
			bzero( &(echoServerAddr.sin_zero), 8);
			int bin = bind(sock, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr));
			int clientStructLen = sizeof(echoClientAddr);
			int tam= recvfrom(sock, Buffer, 1024, 0,(struct sockaddr *) &echoServerAddr, &clientStructLen);
			
			if(tam == -1){
				printf("Error recive\n");
				return -1;
			}
			Buffer[tam] = '\0';
			printf("\nMensaje recibido: %s\n", Buffer);
		}
	}
}

int main(int argc, char *argv[])
{
	int puertoR;
	int puertoE;
	char msj[20];
	int sal;
	sal = 0;
	char *Ip;
	puertoE = 8080;
	puertoR = 8080;
	Ip = "localhost";
	srand(time(NULL));
	msj[0] = 'a' + rand() % (('z' - 'a')+1);
	msj[1] = 'a' + rand() % (('z' - 'a')+1);
	recibe(puertoR,puertoE,msj);
	int x,y;
	if(pid!=0){
		for(x=1;x<=3;x++)
		{
			pid=fork(); 
			if(pid)
			{
				printf("Soy el proceso %d\n",getpid());
				msj[0] = 'a' + rand() % (('z' - 'a')+1);
				msj[1] = 'a' + rand() % (('z' - 'a')+1);
				envia(puertoE,msj,Ip);
				sleep(2);
			}
			else
			{
				printf("1soy el hijo %d, mi padre es %d\n",getpid(),getppid());
				msj[0] = 'a' + rand() % (('z' - 'a')+1);
				msj[1] = 'a' + rand() % (('z' - 'a')+1);
				envia(puertoE,msj,Ip);
				sleep(2);
				for(y=1;y<=2;y++)
				{
					pid=fork();
		 			if(pid)
		 			{
		  				msj[0] = 'a' + rand() % (('z' - 'a')+1);
						msj[1] = 'a' + rand() % (('z' - 'a')+1);
						envia(puertoE,msj,Ip);
		  				sleep(2);
		 			}
		 			else
		 			{
		 				printf("2soy el hijo %d, mi padre es %d\n",getpid(),getppid());
		  				msj[0] = 'a' + rand() % (('z' - 'a')+1);
						msj[1] = 'a' + rand() % (('z' - 'a')+1);
						envia(puertoE,msj,Ip);
		  				sleep(2);
		  				exit(0);
		 			}
				}
				exit(0); 
			}
		}
	exit(0);
	}
	return 0;
}
