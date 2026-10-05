// Autores: Pedro Mariano dos Santos e Rodrigo Rodrigues Tato Gama da Silva.
// CPU dos testes: Apple M4, 10 nucleos fisicos e 10 logicos (4P + 6E).
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char *argv[]) {
    long int tam = 10000000;  // Tamanho padrao: 10 milhoes.
    if (argc > 1)
        tam = atol(argv[1]);

    int aux_thread = 8;  // Use 1, 2, 4 ou 8 neste notebook.
    if (argc > 2)
        aux_thread = atoi(argv[2]);

    omp_set_dynamic(0);  // Mantem o numero de threads solicitado.
    omp_set_max_active_levels(8);  // Permite regioes paralelas aninhadas.

    srand(42);  // Mesma semente nas duas versoes: mesmo vetor de entrada.
    long int *vetor = (long int*)malloc(tam * sizeof(long int));
    if (vetor == NULL) {
        printf("Erro ao alocar memoria para o vetor.\n");
        return 1;
    }

    for (long int i = 0; i < tam; i++)
        vetor[i] = rand() % 1000;

    double inicio = omp_get_wtime();
    mergeSort(vetor, 0, tam - 1, aux_thread);
    double tempo = omp_get_wtime() - inicio;

    printf("Tamanho: %ld | Threads: %d | Tempo: %.6f s\n", tam, aux_thread, tempo);

    free(vetor);
    return 0;
}
