/*Atividade 8 - Mailbox em C
Equipe: Caio Cavalcante, Marcos Levi, Matheus Queiroz*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <process.h>
#include <windows.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

int canal[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
sem_t semaf_send[8];
sem_t semaf_rec[8];

int converte_valor(unsigned char IDproc, unsigned char mens){
	return (IDproc << 8) + mens;
}

void encontra_bytes(int valor, unsigned char *IDproc, unsigned char *mens){
	*IDproc= ((valor >> 8) & 255);
	*mens = (valor & 255);
	return;
}

void send_sync (int *buf, int can){
	canal[can] = *buf;
	sem_post(&semaf_send[can]);
	sem_wait(&semaf_rec[can]);
}

void receive (int *buf, int can){
	sem_wait(&semaf_send[can]);
	*buf = canal[can];
	sem_post(&semaf_rec[can]);
	return;
}

void thread_a(){
	int pkg;
    unsigned char id = 4;
    unsigned char msg = 10;
    pkg = converte_valor (id, msg);
	while(1){
	    Sleep(1000);
		send_sync (&pkg, 0);
	}

}

void thread_b(){
	int pkg;
    unsigned char id = 5;
    unsigned char msg = 20;
    pkg = converte_valor (id, msg);
	while(1){
        //Sleep(1000);
		send_sync (&pkg, 1);
	}
}

void thread_c(){
	int pkg;
    unsigned char id = 6;
    unsigned char msg = 30;

    pkg = converte_valor (id, msg);
	while(1){
        //Sleep(1000);
		send_sync  (&pkg, 2);
	}
}

void thread_d(){
	int pkg;
    unsigned char id = 7;
    unsigned char msg = 40;
    pkg = converte_valor (id, msg);
	while(1){
        //Sleep(1000);
		send_sync  (&pkg, 3);
	}
}

void thread_e(){
	int valor;
	while(1){
		receive(&valor, 4);
		printf("Thread E Recebe: %d\n", valor);
	}
}

void thread_f(){
	int valor;
	while(1){
		receive(&valor, 5);
		printf("Thread F Recebe: %d\n", valor);
	}
}

void thread_g(){
	int valor;
	while(1){
		receive(&valor, 6);
		printf("Thread G Recebe: %d\n", valor);
	}
}

void thread_h(){
	int valor;
	while(1){
		receive(&valor, 7);
		printf("Thread H Recebe: %d\n", valor);
	}
}

void mailbox(){
	int buffer[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int indice = 0;
    int a, b, c, d, i, messageInt;
	unsigned char id, message;

	while(1){
		while (indice < 12){
			receive(&a, 0);
			receive(&b, 1);
			receive(&c, 2);
			receive(&d, 3);
			buffer[indice++] = a;
			buffer[indice++] = b;
			buffer[indice++] = c;
			buffer[indice++] = d;
		}

		indice = 0;
		for (i = 0; i < 12; i++){
			encontra_bytes(buffer[i], &id, &message);
			messageInt = (int)message;
			send_sync(&messageInt, (int)id);
		}
	}
}

void start_thread(){
	long int thread_id;

    #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_a,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_a,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread A.");
            return;
        }
    printf("Thread A() foi criada\n");

    #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_b,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_b,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread B.");
            return;
        }
    printf("Thread B() foi criada\n");

    #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_c,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_c,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread C.");
            return;
        }
    printf("Thread C() foi criada\n");

        #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_d,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_d,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread D.");
            return;
        }
    printf("Thread D() foi criada\n");

        #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_e,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_e,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread E.");
            return;
        }
    printf("Thread E() foi criada\n");

        #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_f,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_f,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread F.");
            return;
        }
    printf("Thread F() foi criada\n");

        #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_g,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_g,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread G.");
            return;
        }
    printf("Thread G() foi criada\n");

        #if defined(__WIN32__)
        if((thread_id = _beginthread(thread_h,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(thread_h,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread H.");
            return;
        }
    printf("Thread H() foi criada\n");
}

void main() {
	long int thread_id;
    int i;

	for(i=0;i<8;i++){
        sem_init(&semaf_send[i],0,0);
    }

	#if defined(__WIN32__)
        if((thread_id = _beginthread(mailbox,4096,NULL)) == (unsigned long)-1)
    #else
        if((thread_id = _beginthread(mailbox,4096,NULL)) == -1)
    #endif // defined
        {
            printf("Nao foi possivel criar a thread mailbox.");
        }
    printf("Thread mailbox foi criada\n");
    start_thread();
    system("PAUSE");
	return;
}
