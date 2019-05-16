/*Atividade 3 - N versões (Voto Majoritário em C)
Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz*/

#include <stdio.h>	
#include <stdlib.h>	
#include <iostream>
#include <errno.h>		
#include <stddef.h>     
#include <process.h>

int canal[6]={-1,-1,-1,-1,-1,-1};

int compara(int *versaoErrada, int vetorvoto[]){
	if ( (vetorvoto[0] == vetorvoto[1] ) && (vetorvoto[1] == vetorvoto[2] ) ){
		*versaoErrada = -1;
		return vetorvoto[0];
	}
	if ( ( vetorvoto[0] == vetorvoto[1] ) && ( vetorvoto[1] != vetorvoto[2] ) ){
		*versaoErrada = 2;
		return vetorvoto[1];
	}
	if ( ( vetorvoto[0]!= vetorvoto[1] ) && ( vetorvoto[1]==vetorvoto[2] ) ){
		*versaoErrada = 0;
		return vetorvoto[2];
	}
	if ( ( vetorvoto[0] == vetorvoto[2] ) && ( vetorvoto[2] != vetorvoto[1] ) ){
		*versaoErrada = 1;
		return vetorvoto[0];
	}
}

void send_assync (int *buf, int can){
	canal[can] = *buf;
}

void receive (int *buf, int can){
	while(canal[can] == -1);
	*buf = canal[can];
	canal[can] = -1;
	return;
}

void thread_code_a(void *threadno)
{
	int voto = 5+5;
	int status;
	send_assync( &voto, 0 );
	receive(&status, 3);
	if (status==1){
		printf("\nVersao TA falhou!");
		return;
	}else {
		printf("\nVersao TA correta!");
		while(1);
	}
}

void thread_code_b(void *threadno)
{
	int voto = 2*5;
	int status;
	send_assync( &voto, 1 );
	receive(&status, 4);
	if (status == 1){
		printf("\nVersao TB falhou!");
		return;
	}else {
		printf("\nVersao TB correta!");
		while(1);
	}
}

void thread_code_c(void *threadno)
{
	int voto = 3+7;
	int status;
	send_assync( &voto, 2 );
	receive(&status, 5);
	if (status == 1){
		printf("\nVersao TC falhou!");
		return;
	}else {
		printf("\nVersao TC correta!");
		while(1);
	}
}

void start_thread()
{
	long int thread_id;
	#if     defined(__WIN32__)
	if ((thread_id = _beginthread(thread_code_a,4096,  NULL)) == (unsigned long)-1)
	#else
	if ((thread_id = _beginthread(thread_code_a,4096,  NULL)) == -1)
	#endif
	    {
	printf("Unable to create thread a.\n");
	return;
	    }
	printf("Created thread a.\n");
	
	    #if     defined(__WIN32__)
	if ((thread_id = _beginthread(thread_code_b,4096, NULL)) == (unsigned long)-1)
	#else
	if ((thread_id = _beginthread(thread_code_b,4096, NULL)) == -1)
	#endif
	    {
	printf("Unable to create thread b.\n");
	return;
    }
	printf("Created thread b.\n");
	
	    #if     defined(__WIN32__)
	if ((thread_id = _beginthread(thread_code_c,4096, NULL)) == (unsigned long)-1)
	#else
	if ((thread_id = _beginthread(thread_code_c,4096, NULL)) == -1)
	#endif
	    {
	printf("Unable to create thread c.\n");
	return;
	    }
	printf("Created thread c.\n");
}

void thread_driver(void *threadno){
	int i;
	int vetorvoto[3];
	int versaoErro;
	int status_erro=1;
	int status_ok=0;

	start_thread();	
	for (i = 0; i < 3; i++)
	receive(&vetorvoto[i], i);
	printf("\nVoto Majoritario : %d\n",compara(&versaoErro, vetorvoto));
	if(versaoErro==-1){
		send_assync(&status_ok,3);
		send_assync(&status_ok,4);
		send_assync(&status_ok,5);
	}
	if(versaoErro==0){
		send_assync(&status_erro,3);
		send_assync(&status_ok,4);
		send_assync(&status_ok,5);
	}
	if(versaoErro==1){
		send_assync(&status_ok,3);
		send_assync(&status_erro,4);
		send_assync(&status_ok,5);
	}
	if(versaoErro==2){
		send_assync(&status_ok,3);
		send_assync(&status_ok,4);
		send_assync(&status_erro,5);
	}
	printf ("\nVersao Errada: %d", versaoErro);
}

void start_driver()
{
	long int thread_id;
	#if     defined(__WIN32__)
	if ((thread_id = _beginthread(thread_driver,4096,  NULL)) == (unsigned long)-1)
	#else
	if ((thread_id = _beginthread(thread_driver,4096,  NULL)) == -1)
	#endif
	    {
	printf("Unable to create thread driver.\n");
	return;
	    }
	printf("Created thread driver.\n");
}

int main(void)
{
	start_driver();
	system("PAUSE");
	return 0;
}
