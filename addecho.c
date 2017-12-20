#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

int main(int argc, char * argv[]) {
    
    //check and filter optional argument values
    int opt;
    int vol, delay;
    
    vol = 4;
    delay = 8000;
    
    while ((opt = getopt(argc, argv, "d:v:")) != -1) {
        switch (opt) {
            case 'd':
                if (optarg < 0)
                {
                    fprintf(stderr, "Not positive integer.");
                    exit(1);
                } else {
                    delay = atoi(optarg);
                }
                break;
            case 'v':
                if (optarg < 0)
                {
                    fprintf(stderr, "Not positive integer.");
                    exit(1);
                } else {
                    vol = atoi(optarg);
                }
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-d delay] [-v volume] <input> <output>\n", argv[0]);
                exit(1);
        }
    }
    if ((argc < 3) || (argc > 7)){
        fprintf(stderr, "Invalid arguments.");
        exit(1);
    }
    
    // Open files
    FILE *input;
    FILE *output;
    input = fopen(argv[optind], "rb");
    output = fopen(argv[optind + 1], "wb");
    
    if (!input)
    {
        fprintf(stderr, "Unable to open source file.");
        exit(1);
    }
    if (!output)
    {
        fprintf(stderr, "Unable to open destination file.");
        exit(1);
    }
    
    //Get length
    unsigned long len;
    fseek(input, 0, SEEK_END);
    len= ftell(input);
    fseek(input, 0, SEEK_SET);
    
    //Header
    char *header;
    header = (char *)malloc(44);
    
    if (!header)
    {
        fprintf(stderr, "Header memory error.");
        exit(1);
    }
    
    fread(header, 44, 1, input);
    header[4] = (header[4] + delay *2);
    header[40] = (header[40] + delay *2);
    for (int i = 0; i < 44; i++){
        fwrite(&header[i], 1, 1, output);
    }
    
    
    //Allocate memory based on delay
    char *buffer;
    buffer = (char *)malloc(delay);
    char *echobuffer;
    echobuffer = (char *)malloc(delay);
    
    if (!buffer)
    {
        fprintf(stderr, "Main buffer memory error.");
        exit(1);
    }
    
    if (!echobuffer)
    {
        fprintf(stderr, "Echo buffer memory error.");
        exit(1);
    }
    
    //Before delay
    unsigned long n;
    n = fread(buffer, delay, 1, input);
    for (int i = 0; i < delay; i++){
        fwrite(&buffer[i], 1, 1, output);
    }
    memcpy(echobuffer, buffer, delay);
    memset(buffer, 0, delay);
    
    //During delay
    while (n == 1){
        char temp;
        n = fread(buffer, delay, 1, input);
        for (int i = 0; i < delay; i++){
            temp = (buffer[i] + (echobuffer[i]/2));
            fwrite(&temp, 1, 1, output);
        }
        memset(echobuffer, 0, delay);
        memcpy(echobuffer, buffer, delay);
        memset(buffer, 0, delay);
    }
    
    //After original ends
    if (delay <= len){
        for (int i = 0; i < delay; i++){
            echobuffer[i] = (echobuffer[i]/2);
            fwrite(&echobuffer[i], 1, 1, output);
        }
    } else {
         short zero = 0;
         for (int i = 0; i < (len - delay); i++){
         fwrite(&zero, 1, 1, output);
         }
    }
    fclose(input);
    fclose(output);
    
    return 0;
}
