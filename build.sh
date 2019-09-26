cc -std=c99 -g -m64 -mavx2 -Wall  -O3 -falign-functions=32 -falign-loops=32 -funroll-loops -o charsearch *.c -x assembler index_scas.s

