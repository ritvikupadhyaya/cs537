//
//  rangesort.c
//  537_p1
//
//  Created by Ritvik Upadhyaya on 13/09/15.
//  Copyright (c) 2015 Ritvik Upadhyaya. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>

//The compare function.
int compareFunction (const void *a, const void *b){
    
    const rec_t *rec1 = a;
    const rec_t *rec2 = b;
    
    return ((int) (rec1 -> key) - (int) (rec2 -> key));
}

int main (int argc, char * argv[]){
    
    //If any of the arguments are missing
    if (argc != 9) {
        fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n");
        exit(1);
    }
    
    
    
    long int lowvalue  = 0;
    unsigned int highvalue = 0;
    //Path of the output File
    char *outputFile   = "/foobar/foo/path";
    //Path of the input file
    char *inputFile   = "/foobar/foo/path";
    
    //inArgs holds each flag's detail.
    int inArg;
    opterr = 0;
    while ((inArg = getopt(argc, argv, "i:o:l:h:")) != -1) {
        switch (inArg) {
            case 'l':
                lowvalue  = atoi(optarg);
                if (lowvalue < 0) {
                    fprintf(stderr, "Error: Invalid range value\n");
                    exit(1);
                }
                
                break;
            case 'h':
                if (strlen(optarg) > 10) {
                    fprintf(stderr, "Error: Invalid range value\n");
                    exit(1);
                }
                highvalue = atoi(optarg);
                break;
            case 'i':
                inputFile = strdup(optarg);
                break;
            case 'o':
                outputFile   = strdup(optarg);
                break;
            default:
                fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n");
                exit(1);
        }
    }
    if (lowvalue > highvalue) {
        fprintf(stderr, "Error: Invalid range value\n");
        exit(1);
    }

    int fd = open(inputFile, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error: Cannot open file %s\n", inputFile);
        exit(1);
    }
    
    struct stat fileStat;
    if(stat(inputFile, &fileStat) < 0){
        fprintf(stderr, "Error: Cannot open file %s\n", inputFile);
        exit(1);
    }
    int fileSize = fileStat.st_size;
    
    int oFd = open(outputFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (oFd < 0) {
        fprintf(stderr, "Error: Cannot open file %s\n", outputFile);
        exit(1);
    }
    
    int counter = 0;
    rec_t* recordArray = (rec_t*) malloc(fileSize);
    if (recordArray == NULL)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(0);
    }
    while ((counter + 1) * sizeof(rec_t) <= fileSize) {
        int reacdReturn;
        reacdReturn = read(fd, &(recordArray[counter]), sizeof(rec_t));
        //The current item in the array
        rec_t * curr = (rec_t*)&recordArray[counter];
        if (curr->key >= lowvalue && curr->key <= highvalue) {
            counter++;
        }
        //EOF
        if (reacdReturn == 0)
            break;
        //If the read failed for Some reason.
        if (reacdReturn < 0) {
            fprintf(stderr, "Read failed!\n");
            free(recordArray);
            close(fd);
            close(oFd);
            exit(1);
        }
    }
    
    qsort(recordArray, counter, sizeof(rec_t), compareFunction);
    
    int SortedFileSize = counter * sizeof(rec_t);
    
    counter = 0;
    
    int rc = write(oFd, &(recordArray[counter]), SortedFileSize);
    if (rc != SortedFileSize) {
        fprintf(stderr, "Write failed!\n");
        free(recordArray);
        close(fd);
        close(oFd);
        exit(1);
    }
    free(recordArray);
    close(fd);
    close(oFd);
    return 0;
}

/*
 
 */













