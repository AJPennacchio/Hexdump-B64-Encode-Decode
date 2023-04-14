/*
 * Project 1 of COMP389: Cryptography
 * Function: hexdump, enc-base64 and dec-base64 handling
 * File Name: fns.c
 */

#include <stdlib.h>
#include <stdio.h>

static const char b64_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
  };

/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/

//inspiration from http://www.dmulholl.com/lets-build/a-hexdump-utility.html
int hexdump(FILE *fp){
  
  char buffer[16];
  int bytes_processed = 0;
  int num_bytes = fread(&buffer, sizeof(char), 16, fp);

  while(num_bytes > 0){
  
    //print current line, converting characters to hex values
    printf("%06X: ", bytes_processed);
    bytes_processed += num_bytes;

    //if no char is being printed, spacers
    for(int i = 0; i < 16; i++){

      if(num_bytes < 16 && i >= num_bytes){
        if(i == 7){
          printf("--  ");
        } else{
          printf("-- ");
        }
        continue;
      }

      //extra space after 8th characer
      if(i == 7){
        printf("%02X  ", (unsigned char)buffer[i]);
      } else{
        printf("%02X ", (unsigned char)buffer[i]);
      }
      
    }
    //add the plaintext onto current line 
    for(int i = 0; i < num_bytes; i++){
      
      if(buffer[i] < 33){
        printf(".");
      } else if (buffer[i] <= 126){
        printf("%c", buffer[i]);
      } else{
        printf("~");
      }
          
    }
    printf("\n");

    //read current line into buffer, number of characters read stores in num_bytes
    num_bytes = fread(&buffer, sizeof(char), 16, fp);

  }

  fclose(fp);
  return 0; 
}

/*
 * read from the specified file or stdin and do enc-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/

//similar solution to https://github.com/jwerle/b64.c/blob/master/encode.c
int enc_base64(FILE *fp){

  //buffer for reading 3 chars into, enc for holding base64 chars
  unsigned char buffer[3];
  unsigned char enc[4];
  int num_bytes = fread(&buffer, sizeof(char), 3, fp);

  while(num_bytes > 0){    

    //if less than 3 bytes read, put NULL character in remaining buffer spaces to override previous reads
    if(num_bytes < 3){
      for(int i = num_bytes; i < 3; i++){
        buffer[i] = '\0';
      }
    }

    //processing up to 3 bytes, and breaking into 4 characters (each is 6 bits) stored in enc
      enc[0] = (buffer[0] & 0b11111100) >> 2; 
      enc[1] = ((buffer[0] & 0b00000011) << 4) + ((buffer[1] & 0b11110000) >> 4);
      enc[2] = ((buffer[1] & 0b1111) << 2) + ((buffer[2] & 0b11000000) >> 6);
      enc[3] = buffer[2] & 0b111111;
    
    //padding if num_bytes is not a multiple of 3. otherwise no padding necessary
    if(num_bytes == 2){
        printf("%c%c%c=", b64_table[ enc[0] ], b64_table[ enc[1] ], b64_table[ enc[2] ]);
    } else if(num_bytes == 1){
        printf("%c%c==", b64_table[ enc[0] ], b64_table[ enc[1] ]);
    } else{
        for(int i = 0; i < 4; i++){
         printf("%c", b64_table[ enc[i] ]);
        }
    }

    //read current line into buffer, number of characters read stores in num_bytes
    num_bytes = fread(&buffer, sizeof(char), 3, fp);
  }

  printf("\n");
  fclose(fp);
  return 0;
}

/*
 * read from the specified file or stdin and do dec-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/

//solution inspired from https://github.com/jwerle/b64.c/blob/master/decode.c
int dec_base64(FILE *fp){
  //buffer for reading 4 base64 chars into, dec for holding decrypted chars
  unsigned char buffer[4];
  unsigned char dec[3];
  int num_bytes = fread(&buffer, sizeof(char), 4, fp);

  while(num_bytes > 0){
  
      //convert base64 characters to index of base64 table
      for(int i = 0; i < num_bytes; i++){
        for(int j = 0; j < 64; j++){
          if(buffer[i] == b64_table[j]){
            buffer[i] = j;
            break;
          } 
          //do not want to process padding
          if(buffer[i] == 0x3D){
            num_bytes--;
            break;
          }
          //cannot decrypt if cipher character is not in base64 table
          else if(j == 63){
            printf("Invalid non-base64 character: %c, cannot further decrypt.\n", buffer[i]);
            return -1;
          }
        }
      }
      //perform decryption, base64 characters converted to 8 bit characters.
      dec[0] = (buffer[0] << 2) + ((buffer[1] & 0b110000) >> 4);
      dec[1] = ((buffer[1] & 0b1111) << 4) + ((buffer[2] & 0b111100) >> 2);
      dec[2] = ((buffer[2] & 0b11) << 6) + buffer[3];
    
    //check size of num_bytes to see if any padding, then print accordingly
    if(num_bytes == 2){
      printf("%c", dec[0]);
    } else if(num_bytes == 3){
      printf("%c%c", dec[0], dec[1]);
    } else if (num_bytes == 4){
      for(int i = 0; i < 3; i++){
        printf("%c", dec[i]);
      }
    }

    //read current line into buffer, number of characters read stores in num_bytes
    num_bytes = fread(&buffer, sizeof(char), 4, fp);
   
  }

  printf("\n");
  fclose(fp);
  return 0; 

}