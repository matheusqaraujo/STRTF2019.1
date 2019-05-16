/*Atividade 6 - Redundância de software com semáforo
Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <process.h>
#include <windows.h>
#include <unistd.h>
#include <semaphore.h>

int canal[6]={-1,-1,-1,-1,-1,-1};
int chanAB = 0, chanBA = 1, chanAC = 2, chanCA = 3, chanBC = 4, chanCB = 5;
sem_t semaf_send[6];

void send_assync (int *buf, int can){
	canal[can] = *buf;
	sem_post(&semaf_send[can]);
}

void receive (int *buf, int can){
	sem_wait(&semaf_send[can]);
	*buf = canal[can];
	return;
}

int fa(){
	return 5;
}

int fb(){
	return 50;
}

int fc(){
	return 1;
}

void *thread_code_a(void *parg){
	int x,y;
	do{
		x = fa();
		printf("TA executando Funcao A\n");
		send_assync(&x,chanAB); //canal AB
		receive(&y,chanCA);     //canal CA
	} while((y>=1)&&(y<=10));
	if (y==-5){
		do{
			x = fa();
			printf("TA executando Funcao A\n");
			send_assync(&x,chanAC); //canal AC
			receive(&y,chanCA);     //canal CA
		}while((y>=1)&&(y<=10));
		while(1){
			fa();
			fb();
			fc();
			printf("TA executando Funcao A,B,C \n");
		}
	}else{
		int status = -5;
		send_assync(&status,chanAB);
        receive(&y,chanBA); //canal BA
		while((y>=50)&&(y<=100)){
			x=fa();
			fc();
			printf("TA executando Funcao A e funcao C tambem!\n");
            send_assync(&x,chanAB); //canal AB
            receive(&y,chanBA);     //canal BA
		}
		while(1){
			fa();
			fb();
			fc();
			printf("TA executando Funcao A,B,C \n");
		}
	}
}

void *thread_code_b(void *parg){
	int x,y;
    receive(&y,chanAB); //canal AB
	while((y>=5)&&(y<=10)){
		x = fb();
		printf("TB executando Funcao B\n");
		send_assync(&x,chanBC); //canal BC
		receive(&y,chanAB);     //canal AB
	}
	if (y==-5){
		do{
			x=fb();
			printf("TB executando Funcao B\n");
			send_assync(&x,chanBA); //canal BA
			receive(&y,chanAB);     //canal AB
		}while((y>=5)&&(y<=10));
		while(1){
			fa();
			fb();
			fc();
			printf("TB executando Funcao A,B,C \n");
		}
	}else{
		int status = -5;
		send_assync(&status,chanBC);
        receive(&y,chanCB); //canal CB
		while((y>=1)&&(y<=10)){
			x=fb();
			fa();
			printf("TB executando Funcao B e funcao A tambem!\n");
			send_assync(&x,chanBC); //canal BC
			receive(&y,chanCB);     //canal CB
		}
		while(1){
			fa();
			fb();
			fc();
			printf("TB executando Funcao A,B,C \n");
		}
	}
}

void *thread_code_c(void *parg){
	int x,y;

    receive(&y,chanBC); //canal BC
	while((y>=50)&&(y<=100)){
		x = fc();
		printf("TC executando Funcao C\n");
		send_assync(&x,chanCA); //canal CA
		receive(&y,chanBC);     //canal BC
	}
	if (y==-5){
		do{
			x = fc();
			printf("TC executando Funcao C\n");
			send_assync(&x,chanCB); //canal CB
			receive(&y,chanBC);     //canal BC
		}while((y>=50)&&(y<=100));
		while(1){
			fa();
			fb();
			fc();
			printf("TC executando Funcao A,B,C \n");
		}
	}else{
		int status = -5;
		send_assync(&status,chanCA);
        receive(&y,chanAC); //canal AC
		while((y>=5)&&(y<=10)){
			x=fc();
			fb();
			printf("TC executando Funcao C e funcao B tambem!\n");
			send_assync(&x,chanCA); //canal CA
			receive(&y,chanAC);     //canal AC
		}
		while(1){
			fa();
			fb();
			fc();
			printf("TC executando Funcao A,B,C \n");
		}
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


int main(int argc, char** argv) {
    int i;

    for(i=0;i<6;i++){
        sem_init(&semaf_send[i],0,0);
    }
	start_thread();
    system("pause");
	return 0;
}
