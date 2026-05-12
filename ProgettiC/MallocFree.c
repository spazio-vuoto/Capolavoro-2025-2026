#include <stdio.h>
#include <stdlib.h>

#define MEMORIA 1024
char memoria[MEMORIA];

typedef struct Blocco{
    size_t spazio;
    int libero;
    struct Blocco *successivo;
}Blocco;

Blocco *primoBlocco = NULL;

void inizializza_memoria(){
    primoBlocco = (Blocco*)memoria;
    primoBlocco->spazio = MEMORIA - sizeof(Blocco);
    primoBlocco->libero = 1;
    primoBlocco->successivo = NULL;
}

void* my_malloc(size_t size) {
    Blocco* curr = primoBlocco;

    while(curr){
        if(curr->libero && curr->spazio >= size){

            if (curr->spazio > size + sizeof(Blocco)) {
                Blocco* new_block = (Blocco*)((char*)curr + sizeof(Blocco) + size);

                new_block->spazio = curr->spazio - size - sizeof(Blocco);
                new_block->libero = 1;
                new_block->successivo = curr->successivo;

                curr->successivo = new_block;
                curr->spazio = size;
            }

            curr->libero = 0;
            return (char*)curr + sizeof(Blocco);
        }

        curr = curr->successivo;
    }

    return NULL;
}

void my_free(void* ptr){
    if (!ptr) return;

    Blocco* block = (Blocco*)((char*)ptr - sizeof(Blocco));
    block->libero = 1;

    // merge blocchi liberi consecutivi
    Blocco* curr = primoBlocco;

    while(curr && curr->successivo){
        if(curr->libero && curr->successivo->libero){
            curr->spazio += sizeof(Blocco) + curr->successivo->spazio;
            curr->successivo = curr->successivo->successivo;
        }else{
            curr = curr->successivo;
        }
    }
}

void stampa_memoria() {
    Blocco* curr = primoBlocco;
    int i = 0;

    while (curr) {
        printf("Blocco %d: %zu bytes, %s\n",
            i++,
            curr->spazio,
            curr->libero ? "LIBERO" : "OCCUPATO");

        curr = curr->successivo;
    }
}

int main() {
    inizializza_memoria();

    void* a = my_malloc(100);
    void* b = my_malloc(200);

    printf("Allocati A e B\n");

    my_free(a);
    printf("Liberato A\n");

    void* c = my_malloc(50);
    printf("Allocato C\n");

    return 0;
}
