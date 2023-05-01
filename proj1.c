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
  // usage();
  // Check command arg first
  // Check if file arg is valid and then open file
  // Then pass file ptr to methods

//  printf("arg 0: (%s)\n", argv[0]);
//  printf("arg 1: (%s)\n", argv[1]);
//  printf("arg 2: (%s)\n", argv[2]);
//  printf("argc: (%d)\n", argc);
//  printf("argv: (%s)\n", *argv);
  char hdOption[] = "hexdump";
  char ebOption[] = "enc-base64";
  char dbOption[] = "dec-base64";

  if (strcmp(argv[1], "\0") == 0 || argv[1] == NULL || strcmp(argv[0], "\0") == 0 || argv[0] == NULL) {
      return 1;
  }

  FILE *fp;

  if (argv[2] == 0 && stdin == NULL) {
      printf("no file argument supplied\n");
      return 1;
  } else if (argv[2] != 0) {
      fp = fopen(argv[2], "rb");
  } else {
      fp = stdin;
  }

  if (fp == NULL) {
      printf("file not found\n");
      return 1;
  }


  if (strcmp(argv[1], hdOption) == 0) {
      hexdump(fp);
  } else if (strcmp(argv[1], ebOption) == 0) {
      enc_base64(fp);
  } else if (strcmp(argv[1], dbOption) == 0) {
      dec_base64(fp);
  } else {
      return 1;
  }

  return 0;
}

