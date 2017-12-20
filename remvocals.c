#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char * argv[]) {
    FILE *source;
    FILE *dest;
    char *buffer;
    unsigned long len;
    
    //Open file
    source = fopen(argv[1], "rb");
    dest = fopen(argv[2], "wb");
    if (!source)
    {
        fprintf(stderr, "Cannot open source file.");
        exit(1);
    }
    if (!dest)
    {
        fprintf(stderr, "Cannot open destination file.");
        exit(1);
    }
    
    //Check argument length
    if (argc != 3)
    {
        fprintf(stderr, "Need to input two arguments.");
        exit(1);
    }
    
    //Get file length and check if even
    fseek(source, 0, SEEK_END);
    len= ftell(source);
    fseek(source, 0, SEEK_SET);
    if (len % 2 != 0){
        fprintf(stderr, "Not an even byte file.");
        exit(1);
    }
    
    //Allocate memory
    buffer= malloc(len+1);
    if (!buffer)
    {
        fprintf(stderr, "Memory error.");
        exit(1);
    }
    
    //Read file into buffer
    fread(buffer, len, 1, source);
    fclose(source);
    
    //write header to destination file
    for (int i = 0; i < 44; i++){
        fwrite(&buffer[i], 1, 1, dest);
    }
    
    // alter the values in buffer and write to destination
    for (int i = 44; i < len; i += 4){
        short temp1;
        short temp2;
        temp1 = (buffer[i] - buffer[i + 2])/2;
        temp2 = (buffer[i+1] - buffer[i + 3])/2;
        fwrite(&temp1, 1, 1, dest);
        fwrite(&temp2, 1, 1, dest);
        fwrite(&temp1, 1, 1, dest);
        fwrite(&temp2, 1, 1, dest);
    }
    fclose(dest);
}