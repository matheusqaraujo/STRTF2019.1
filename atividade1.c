/*Atividade 1
Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz*/

#include <stdio.h>
#include <stdlib.h>
#include <process.h>

int flagA=1,flagB=0,flagC=0;
char listachar[30];
int indice=0;

void threadA(void *threadno){
	while(indice<=27){
		while(flagA==0);
		listachar[indice]='A';
		printf(listachar);
		printf("  -  %d",indice);
		printf("\n");
		indice++;
		flagA=0;
		flagB=1;
	}
	return;	
}

void threadB(void *threadno){
	while(indice<=28){
		while(flagB==0);
		listachar[indice]='B';
		printf(listachar);
		printf("  -  %d",indice);
		printf("\n");
		indice++;
		flagB=0;
		flagC=1;
	}
	return;	
}

void threadC(void *threadno){
	while(indice<=29){
		while(flagC==0);
		listachar[indice]='C';
		printf(listachar);
		printf("  -  %d",indice);
		printf("\n");
		indice++;
		flagC=0;
		flagA=1;	
	}
	return;	
}

void criaThreadA(){
	long int thread_A;
	#if defined(_WIN32_)
	if((thread_A=_beginthread(threadA,4096,NULL))==(unsigned long)-1)
	#else
	if((thread_A=_beginthread(threadA,4096,NULL))==-1)
	#endif
	{
		printf("Unable to create thread A.\n");
		return;
	}
	//printf("Created thread A.\n");		
}

void criaThreadB(){
	long int thread_B;
	#if defined(_WIN32_)
	if((thread_B=_beginthread(threadB,4096,NULL))==(unsigned long)-1)
	#else
	if((thread_B=_beginthread(threadB,4096,NULL))==-1)
	#endif
	{
		printf("Unable to create thread B.\n");
		return;
	}
	//printf("Created thread B.\n");	
}

void criaThreadC(){
	long int thread_C;
	#if defined(_WIN32_)
	if((thread_C=_beginthread(threadC,4096,NULL))==(unsigned long)-1)
	#else
	if((thread_C=_beginthread(threadC,4096,NULL))==-1)
	#endif
	{
		printf("Unable to create thread C.\n");
		return;
	}
	//printf("Created thread C.\n");	
}

int main(int argc, char *argv[]){ 
  printf(listachar);
  criaThreadA();
  criaThreadB();
  criaThreadC();
  system("pause");
  //return 0;
}