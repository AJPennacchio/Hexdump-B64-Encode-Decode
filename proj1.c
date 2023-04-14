/*
 * Project 1 of COMP389: Cryptography
 * Function: commandline parsing and file reading
 * File Name: proj1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fns.h"


void usage(char *argv[]){
  
  

  FILE* fp = fopen(argv[2], "r");

 
  if(strcmp("hexdump", argv[1]) == 0){
    hexdump(fp);
  } else if(strcmp("enc-base64", argv[1]) == 0){
    enc_base64(fp);
  } else if(strcmp("dec-base64", argv[1]) == 0){
    dec_base64(fp);
  } else {
    printf("Error: function not found. \n");
    printf("The commandline syntax for proj 1 is as follows:\n");
    printf("    proj1 {hexdump|enc-base64|dec-base64} [file]\n");
  }

  exit(1);
}

int main(int argc, char *argv[]){

  if(argc != 3){
    printf("Error: expected 2 parameters but got %d\n", argc - 1);
    return -1;
  }
  usage(argv);

  return 0;
}

