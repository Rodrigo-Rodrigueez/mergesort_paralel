#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>
#include <omp.h>

void merge(long int vetor[], long int comeco, long int meio, long int fim) {
    long int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
    long int *vetAux;
    vetAux = (long int*)malloc(tam * sizeof(long int));
    if(vetAux == NULL) {
        printf("Erro ao alocar memória para o vetor auxiliar.\n");
        exit(1);
    }

    while(com1 <= meio && com2 <= fim){
        if(vetor[com1] <= vetor[com2]) {
            vetAux[comAux] = vetor[com1];
            com1++;
        } else {
            vetAux[comAux] = vetor[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= meio){  //Caso ainda haja elementos na primeira metade
        vetAux[comAux] = vetor[com1];
        comAux++;
        com1++;
    }

    while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
        vetAux[comAux] = vetor[com2];
        comAux++;
        com2++;
    }

    for(comAux = comeco; comAux <= fim; comAux++){    //Move os elementos de volta para o vetor original
        vetor[comAux] = vetAux[comAux-comeco];
    }
    
    free(vetAux);
}

void mergeSort(long int vetor[], long int comeco, long int fim, long int aux_thread) {
    if (comeco < fim) {
        long int meio = comeco + (fim - comeco) / 2;

        if (aux_thread > 1) {
            #pragma omp parallel sections num_threads(2)
            {
                #pragma omp section
                mergeSort(vetor, comeco, meio, aux_thread/2);
                #pragma omp section
                mergeSort(vetor, meio+1, fim, aux_thread/2);
            }
        } else {
            mergeSort(vetor, comeco, meio, aux_thread);
            mergeSort(vetor, meio+1, fim, aux_thread);
        }

            merge(vetor, comeco, meio, fim);
    }
}

int main (int argc, char *argv[]){
    long int tam = 10000000;  // Tamanho padrão: 10 milhões
    if(argc > 1)
        tam = atol(argv[1]);  // Ou o tamanho passado na linha de comando

    srand(time(NULL));
    long int *vetor = (long int*)malloc(tam * sizeof(long int));
    if(vetor == NULL) {
        printf("Erro ao alocar memória para o vetor.\n");
        exit(1);
    }
    long int comeco = 0, fim = tam - 1;
    for(long int i=0; i<tam; i++)
        vetor[i] = rand() % 1000;

    omp_set_max_active_levels(8); // Permite parallel dentro de parallel (até 2^8 = 256 threads)
    int aux_thread = omp_get_max_threads(); // Get the maximum number of threads available

    double inicio = omp_get_wtime();
    mergeSort(vetor, comeco, fim, aux_thread);
    double tempo = omp_get_wtime() - inicio;

    printf("Tamanho: %ld | Threads: %d | Tempo: %.4f s\n", tam, omp_get_max_threads(), tempo);

    free(vetor);
    return 0;
}