#include <stdio.h>
#include <string.h>

typedef char byte;

void serialize(const char * filename, const size_t size, void * data) {
    FILE * file = fopen(filename, "w");
    byte * stream = (byte *) data; 
    fwrite(&size, sizeof(size), 1, file);
    fwrite(stream, sizeof(byte), size, file);
    fclose(file);
}

void deserialize(const char * filename, void * data) {
    FILE * file = fopen(filename, "r");
    byte * stream = (byte *) data;
    size_t size;
    fread(&size, sizeof(size), 1, file);
    fread(stream, sizeof(byte), size, file);
    fclose(file); 
}

void file_delete(const char * filename) {
    if (remove(filename)) {
        fprintf(stderr, "File \"%s\" could not be deleted.", filename);
    }
}
