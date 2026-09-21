#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

int qtde_primos;
int thread_count;

// estrtura de dados pra armazenar intervalo e quantidade de primos em cada thread
typedef struct {
    int inicio;
    int fim;
    int qtde_primos;
} dados_thread;

// verifica se o numero é primo
int verifica_primo (int num){
    if (num == 0){
        return 0;
    }
    if(num == 1){
        return 0;
    }
    if(num == 2){
        return 1;
    }
    if(num % 2 == 0){
        return 0;
    }
    for(int i = 2; i < num; i++){
        if(num % i == 0){
            return 0;
        }
    }
    return 1;
}

// função que a thread utiliza para verificar primos
void* conta_primos(void* argc) {
    dados_thread* dados = (dados_thread*) argc;

    printf("Thread iniciou: %d - %d\n",
       dados->inicio,
       dados->fim);

    for(int i = dados->inicio; i <= dados->fim; i++){
        if(verifica_primo(i)){
            dados->qtde_primos++;
        }
    }

    printf("Thread terminou: %d - %d | Primos: %d\n",
       dados->inicio,
       dados->fim,
       dados->qtde_primos);
    return NULL;
}

int main(int argc, char* argv[]){
    pthread_t* thread_handles;
    int limite = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);
    dados_thread dados[thread_count];
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    int intervalo = limite/thread_count;



    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for(int i=0; i < thread_count; i++) {
        dados[i].inicio = i * intervalo + 1;
        if(i == thread_count - 1){
            dados[i].fim = limite;
        }else {
            dados[i].fim = (i + 1) * intervalo;
        }
        dados[i].qtde_primos = 0;

        printf("Criando thread: %d: intervalo %d - %d \n",
            i,
            dados[i].inicio,
            dados[i].fim);
      
        pthread_create(&thread_handles[i], NULL, conta_primos, &dados[i]);
    }

    for(int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);  
    }      
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Tempo: %.6f segundos\n", tempo);
    int qtde_primos = 0;

    for(int i = 0; i < thread_count; i++) {
        qtde_primos += dados[i].qtde_primos;
    }

    printf("Quantidade números primos de 1 a %d: %d \n", limite, qtde_primos);
    free(thread_handles);
}