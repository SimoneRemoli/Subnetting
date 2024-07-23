#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//You need to take an IP address as input and translate it into an equivalent network address - using appropriately
//bitwise operations via the mask. 

#define fflush(stdin) while ((getchar()) != '\n')

struct nodo
{
	char netmask_binary[128];
};
void to_binary(struct nodo*, char*, int*);
void convertion(char*, struct nodo*, int*);
int check(int*,struct nodo*);
int* _from_binary_string_to_decimal(char*,int,int,int,int*);
int indexer = 0;
int cont = 4;
int netmask;
int ret;
int main(int argc, char **argv)
{
	char *address = (char*)malloc(sizeof(char)*200); //use malloc to function memcpy
	int *array = (int*)malloc(sizeof(int)*cont);

	struct nodo* IP = malloc(sizeof(struct nodo));
	char buffer[1024];
	printf("Insert IP and netmask (Ex. 192.168.1.2/24) : ");
	if(fgets(buffer,1024,stdin))
		buffer[strcspn(buffer, "\n")] = 0;
	memcpy(address, buffer, strlen(buffer));
	to_binary(IP,address,array);
	ret = check(array,IP);
	if(ret == -1)
	{
		printf("Mask out of range [*] - aborted. \n");
		return 0;
	}
	if(ret==-2)
	{
		printf("Number out of range [0-255]->8bit. [*] - aborted \n");
		return 0;
	}
	return 0;
}
void to_binary(struct nodo* IP, char *address,int *array)
{
	char *token;
	int lon = strlen(address);
	token = strtok(address, ".");
	while(token!=NULL)
	{
		for(int i=0;i<strlen(token);i++)
			if(*(token+i)=='/')
				token = strtok(token, "/");
		convertion(token, IP, array);
		token = strtok(NULL,".");
	}
	return;
}
void convertion(char *token, struct nodo* IP, int *array)
{
	int val = (int)strtol(token,NULL,10);
	if(indexer==4)
		netmask = val;
	else
	{
		*(array + indexer) = val;
		indexer = indexer + 1;
	}
}
int check(int *a,struct nodo* IP)
{
	int *temp = a, contatore = cont;
	int *vector  =  malloc(sizeof(int)*4);
	int* network_addresses = malloc(sizeof(int)*4);
	int num_uni = netmask;
	if(netmask>32)
		return -1;
	int *b=a;
	while(cont--)
		if((*b++)>255)
			return -2;
	char str_net_bin[35]={0};
	while(netmask--) 
		strcat(str_net_bin, "1");
	for(int i=0;i<32;i++)
		if(str_net_bin[i]!='1')
				str_net_bin[i]='0';
	memcpy(IP->netmask_binary, str_net_bin,1024);
	vector = _from_binary_string_to_decimal(str_net_bin,0,7,0,vector); //from 0 to 7 pointer save value in 0-index vector.
	vector = _from_binary_string_to_decimal(str_net_bin,8,15,1,vector);
	vector = _from_binary_string_to_decimal(str_net_bin,16,23,2,vector);
	vector = _from_binary_string_to_decimal(str_net_bin,24,31,3,vector);
	puts("");
	printf("     Address    = %d  .  %d  .  %d  .  %d \n", *(temp+0),*(temp+1),*(temp+2),*(temp+3));
	printf("Netmask decimal = %d.%d.%d.%d \n", vector[0],vector[1],vector[2],vector[3]);
	printf("     Netmask    = %s \n", IP->netmask_binary);
	network_addresses[0] = *(temp+0)&vector[0];
	network_addresses[1] = *(temp+1)&vector[1];
	network_addresses[2] = *(temp+2)&vector[2];
	network_addresses[3] = *(temp+3)&vector[3];
	puts("---------------------------------------------------");
	printf("Network address = %d.%d.%d.%d \n", network_addresses[0],network_addresses[1],network_addresses[2],network_addresses[3]);
	return 0;
}
int* _from_binary_string_to_decimal(char* str_net_bin,int start,int finish,int index,int* vector)
{
	int indice = 0;
	int valore_decimale=0, n=6;
	int num = 0,power=1;
	for(int i=start;i<finish+1;i++)
	{
		num = str_net_bin[i] - '0';
		for(int j=n;j>=0;j--)
		{
			power *= 2;
		}
		valore_decimale += power*num;
		*(vector + index) = valore_decimale;
		n = n - 1;
		power = 1;
	}
	return vector;
}