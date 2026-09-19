#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

int qtde_primos;
int thread_count;

typedef struct {
    int inicio;
    int fim;
} dados_thread;

int verifica_primo (int num){
    if(num == 1){
        return 0;
    }
    if(num == 2){
        return 1;
    }
if(num % 2 == 0){
        return 0;
    }
    int n = sqrt(num);
    for(int i = 2; i < n; i++){
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]){
    pthread_t* thread_handles;
    int limite = strtol(argv[1], NULL, 10);
    thread_count = strtol(argv[2], NULL, 10);

    thread_handles = malloc(thread_count*sizeof(pthread_t));

    int intervalo = limite/thread_count;

    for(int i=0; i < thread_count; i++) {

        inicio = i * intervalo + 1;
        if(i == thread_count - 1){
            fim = limite;
        }else {
            fim = (i + 1) * intervalo;
        }
        
    }
}