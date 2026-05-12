#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compress(FILE *file, FILE *output){
    int c = fgetc(file);
    if(c == EOF){
        return;
    }

    int c2;
    int counter = 1;

    while((c2 = fgetc(file)) != EOF){ //restituisve EOF fgetc() quando temina il file (End Of File)
        if(c2 == c){
            counter++;
            if(counter == 255){ //serve per non superara la lunghezza di 1 byte
                fputc(c, output);
                fputc(255, output);
                counter = 0;
            }
        }else{
            if(counter > 0){
                fputc(c, output);
                fputc(counter, output);
            }
            c = c2;
            counter = 1;
        }
    }
    fputc(c, output);
    fputc(counter, output);
}

void decompress(FILE *file, FILE *output){
    while(1){
        int c = fgetc(file);

        if(c == EOF){
            break;
        }

        int count = fgetc(file);

        if(count == EOF){
            break;
        }

        for(int i = 0; i < count; i++){
            fputc(c, output);
        }
    }
}

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Usa: %s compress|decompress  file \n", argv[0]);
        exit(-1);
    }

    char *inputFileName = argv[2];

    FILE *file = fopen(argv[2], "rb");
    if(file == NULL){
        printf("File non trovato");
        exit(-1);
    }

    char *ext = strrchr(inputFileName, '.'); // cerca l'ultimo '.' nel nome
    if(ext == NULL){
        printf("File senza estensione, impossibile determinare il tipo di output\n");
        fclose(file);
        exit(-1);
    }

    char outputFileName[256];
    snprintf(outputFileName, sizeof(outputFileName), "Output%s", ext);

    FILE *output = fopen(outputFileName, "wb");
    if(output == NULL){
        printf("Errore nella creazione del file di output\n");
        fclose(file);
        exit(-1);
    }

    if(!strcmp(argv[1], "compress")){ // ! perchè returna 0 se la stringa è uguale
        compress(file, output);
    }else if(!strcmp(argv[1], "decompress")){
        decompress(file, output);
    }else{
        printf("Usa: %s compress|decompress \n", argv[0]);
        fclose(file);
        fclose(output);
        exit(-1);
    }

    fclose(file);
    fclose(output);

    return (0);
}