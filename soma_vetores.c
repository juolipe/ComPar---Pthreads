#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

int qtde_primos;
int thread_count;

// estrutura para alocar vetores e intervalo (dados da thread)
typedef struct {
    int inicio;
    int fim;
    float *A;
    float *B;
    float *C;
} dados_thread;

// função C[i] = A[i] + B[i]
void* soma_vetores(void* argc) {
    dados_thread* dados = (dados_thread*) argc;

    printf("Thread iniciou: %d - %d\n",
       dados->inicio,
       dados->fim);

    for(int i = dados->inicio; i <= dados->fim; i++){
        dados->C[i] = dados->A[i] + dados->B[i];
    }

    printf("Thread terminou: %d - %d\n",
       dados->inicio,
       dados->fim);
    return NULL;
}

int main(int argc, char* argv[]){
    pthread_t* thread_handles;
    int n = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);
    dados_thread dados[thread_count];
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    float *A;
    float *B;
    float *C;

    // alocação da memória pra cada vetor
    A = malloc(n * sizeof(float));
    B = malloc(n * sizeof(float));
    C = malloc(n * sizeof(float));

    // escrever cada vetor 
    for(int i = 0; i < n; i++) {
        A[i] = i;
        B[i] = i;
    }

    int intervalo = n/thread_count;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for(int i=0; i < thread_count; i++) {
        dados[i].inicio = i * intervalo;
        if(i == thread_count - 1){
            dados[i].fim = n - 1;
        }else {
            dados[i].fim = (i + 1) * intervalo - 1;
        }
        dados[i].A = A; 
        dados[i].B = B;
        dados[i].C = C;
        printf("Criando thread: %d: intervalo %d - %d \n",
            i,
            dados[i].inicio,
            dados[i].fim);
        // thread criada
        pthread_create(&thread_handles[i], NULL, soma_vetores, &dados[i]);
    }

    for(int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);  
    }      

    clock_gettime(CLOCK_MONOTONIC, &fim);

    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;
    printf("Tempo: %.6f segundos\n", tempo);

    /*for(int i = 0; i < n; i++) {
        printf("C[%d] = %.2f\n", i, C[i]);
    }*/

    free(thread_handles);
    free(A);
    free(B);
    free(C);
}