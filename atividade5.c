/*Atividade 5 - Redundância de software
Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <process.h>
#include <windows.h>
#include <unistd.h>

int canal[6]={-1,-1,-1,-1,-1,-1};
int chanAB = 0, chanBA = 1, chanAC = 2, chanCA = 3, chanBC = 4, chanCB = 5;

void send_assync (int *buf, int can){
	canal[can] = *buf;
}

void receive (int *buf, int can){
	while(canal[can] == -1);
	*buf = canal[can];
	canal[can] = -1;
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
			x=fa();
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
		do{
			x=fa();
			fc();
			printf("TA executando Funcao A e funcao C tambem!\n");
            receive(&y,chanBA);     //canal BA
			send_assync(&x,chanAB); //canal AB

		}while((y>=50)&&(y<=100));
		while(1){
			fa();fb();fc();
			printf("TA executando Funcao A,B,C \n");
		}
	}
}

void *thread_code_b(void *parg){
	int x,y;

	do{
		receive(&y,chanAB); //canal AB
		x = fb();
		printf("TB executando Funcao B\n");
		send_assync(&x,chanBC); //canal BC

	} while((y>=5)&&(y<=10));
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
		do{
			x=fb();
			fa();
			printf("TB executando Funcao B e funcao A tambem!\n");
			receive(&y,chanCB);     //canal CB
			send_assync(&x,chanBC); //canal BC

		}while((y>=1)&&(y<=10));
		while(1){
			fa();fb();fc();
			printf("TB executando Funcao A,B,C \n");
		}
	}
}

void *thread_code_c(void *parg){
	int x,y;

	do{
		receive(&y,chanBC); //canal BC
		x = fc();
		printf("TC executando Funcao C\n");
		send_assync(&x,chanCA); //canal CA

	} while((y>=50)&&(y<=100));
	if (y==-5){
		do{
			x=fc();
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
		do{
			x=fc();
			fb();
			printf("TC executando Funcao C e funcao B tambem!\n");
			receive(&y,chanAC);     //canal AC
			send_assync(&x,chanCA); //canal CA
		}while((y>=5)&&(y<=10));
		while(1){
			fa();fb();fc();
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
	start_thread();
    system("pause");
	return 0;
}
