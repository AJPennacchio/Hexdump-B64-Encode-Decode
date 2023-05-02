/*
 * Project 1 of COMP389: Cryptography
 * Function: commandline parsing and file reading
 * File Name: proj1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fns.h"

void usage(){
  printf("The commandline syntax for proj 1 is as follows:\n");
  printf("    proj1 {hexdump|enc-base64|dec-base64} [file]\n");
  exit(1);
}

int main(int argc, char *argv[]){

  // Check command arg first
  // Check if file arg is valid and then open file
  // Then pass file ptr to methods
    
  char hdOption[] = "hexdump";
  char ebOption[] = "enc-base64";
  char dbOption[] = "dec-base64";
  FILE *fp;

    //if passing a file
    if(argc == 3){
        fp = fopen(argv[2], "rb");

        //check to see if file can be opened
         if (fp == NULL) {
            printf("Error: File not found. \n");
            return 1;
         }
        
    //if using stdin
    } else if(argc == 2){
         fp = stdin;

    //else generate an error
    } else{
        printf("Error: Improper number of parameters supplied. \n"); 
        return 1;
    }

    //call function based on parameters supplied or generate error
    if (strcmp(argv[1], hdOption) == 0) {
      hexdump(fp);
    } else if (strcmp(argv[1], ebOption) == 0) {
      enc_base64(fp);
    } else if (strcmp(argv[1], dbOption) == 0) {
      dec_base64(fp);
     } else {
        printf("Error: Invalid function. \n");
        return 1;
    }


  return 0;
    
}

