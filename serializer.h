#include <stdio.h>
#include <string.h>

typedef char byte;

void serialize(const char * filename, void * data, int size) {
    FILE * file = fopen(filename, "w");
    byte * stream = (byte *) data; 
    for (int i = 0; i < size; ++i) { 
        putc(stream[i], file);
    }
    fclose(file);
}

void deserialize(const char * filename, void * data, int size) {
    FILE * file = fopen(filename, "r");
    byte * stream = (byte *) data;
    for (int i = 0; i < size && file != NULL; ++i) {
        stream[i] = fgetc(file);
    }
    fclose(file);
}

void file_delete(const char * filename) {
    if (remove(filename)) {
        fprintf(stderr, "File \"%s\" could not be deleted.", filename);
    }
}
