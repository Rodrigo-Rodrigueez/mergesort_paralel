# Códigos simplificados

```sh
make
./build/merge_sort_serial 10000000
./build/merge_sort_paralelo 10000000 8
```

O serial usa OpenMP apenas para o cronômetro; a ordenação é sequencial.
O paralelo recebe tamanho e número de threads (use 1, 2, 4 ou 8).
As duas versões usam srand(42) para gerar a mesma entrada.
Os códigos instrumentados usados no relatório foram preservados em experimento/.
O arquivo merge_sort.c original continua inalterado.
