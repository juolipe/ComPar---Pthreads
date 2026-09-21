#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    long inicio;
    long fim;
    unsigned int seed;
    long pontos_circulo;
} dados_thread;

void* calcula_pi(void* arg) {

    dados_thread* dados = (dados_thread*) arg;

    dados->pontos_circulo = 0;

    for (long i = dados->inicio; i <= dados->fim; i++) {

        double x = (double) rand_r(&dados->seed) / RAND_MAX;
        double y = (double) rand_r(&dados->seed) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            dados->pontos_circulo++;
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    long N = strtol(argv[1], NULL, 10);
    int num_threads = strtol(argv[2], NULL, 10);

    if (N <= 0 || num_threads <= 0) {
        printf("N e num_threads devem ser maiores que zero.\n");
        return 1;
    }

    pthread_t* thread_handles =
        malloc(num_threads * sizeof(pthread_t));

    dados_thread* dados =
        malloc(num_threads * sizeof(dados_thread));

    if (thread_handles == NULL || dados == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(thread_handles);
        free(dados);
        return 1;
    }

    long intervalo = N / num_threads;

    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < num_threads; i++) {

        dados[i].inicio = i * intervalo;

        if (i == num_threads - 1) {
            dados[i].fim = N - 1;
        } else {
            dados[i].fim = (i + 1) * intervalo - 1;
        }

        dados[i].seed =
            (unsigned int) time(NULL) + (unsigned int) i;

        dados[i].pontos_circulo = 0;

        pthread_create(
            &thread_handles[i],
            NULL,
            calcula_pi,
            &dados[i]
        );
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    long pontos_circulo = 0;

    for (int i = 0; i < num_threads; i++) {
        pontos_circulo += dados[i].pontos_circulo;
    }

    double pi =
        4.0 * (double) pontos_circulo / (double) N;

    double tempo =
        (fim.tv_sec - inicio.tv_sec) +
        (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Numero de pontos: %ld\n", N);
    printf("Numero de threads: %d\n", num_threads);
    printf("Pontos dentro do circulo: %ld\n", pontos_circulo);
    printf("Valor estimado de PI: %.10f\n", pi);
    printf("Tempo: %.6f segundos\n", tempo);

    free(thread_handles);
    free(dados);

    return 0;
}