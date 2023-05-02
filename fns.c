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
int hexdump(FILE *fp) {

    rewind(fp);
    int numBytesRead;
    int currLine = 0;
    char *inputBuffer = (char *)malloc(sizeof(char) * 16);
    numBytesRead = fread(inputBuffer, sizeof(char), 16, fp);
    while (numBytesRead != 0) {
        // 16 bytes (1 hexdump line) at a time
        // we malloc and free here to stop overwrite btwn. iteration scopes
        printf("%06x: ", currLine * 16);
        char *hexBuffer = (char *)malloc(32);

        for (int j = 0; j < 16; j++) {
            // check for padding
            if (numBytesRead < 16 && j > numBytesRead - 1) {
                printf("-- ");
                snprintf(&hexBuffer[j * 2], 3, "--");
            } else {
                // convert each byte to hex
                printf("%02hhx ", (unsigned char) inputBuffer[j]);
                snprintf(&hexBuffer[j * 2], 3, "%02hhx", (unsigned char) inputBuffer[j]);
            }

            // print extra space at middle
            if (j == 7) {
                printf(" ");
            }
        }

        // ASCII (use snprintf to store formatted hex into buffer)
        printf(" ");
        for (int j = 0; j < 32; j += 2) {
            char *temp = (char *)malloc(2);
            temp[0] = hexBuffer[j];
            temp[1] = hexBuffer[j + 1];
            if (temp[0] == '-') {
                printf("%c", 32);
                free(temp);
                continue;
            }
            int hexInt = (int)strtol(temp, NULL, 16);
            if (hexInt < 32) {
                printf(".");
            } else if (hexInt > 127) {
                printf("~");
            } else if (hexInt == 127) {
                printf(".");
            } else {
                char value = hexInt;
                printf("%c", value);
            }

            free(temp);
        }
        free(hexBuffer);
        printf("\n");
        numBytesRead = fread(inputBuffer, sizeof(char), 16, fp);
        currLine++;
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
    int count = 0;

    while(num_bytes > 0){
        count++;
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

        if (count % 16 == 0 || num_bytes <= 0) {
            printf("\n");
        }
    }

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
int dec_base64(FILE *fp) {
    //buffer for reading 4 base64 chars into, dec for holding decrypted chars
    unsigned char buffer[4];
    unsigned char dec[3];
    int letter = 0;
    int padding = 0;

    for(int i = 0; i < 4; i++){
        letter = getc(fp);
       
        while(letter == '\n' || letter == ' '){
            letter = getc(fp);
        }
        buffer[i] = letter;
    }
    
    while(letter != EOF){
        //convert base64 characters to index of base64 table
        for(int i = 0; i < 4; i++){

            for(int j = 0; j < 64; j++){
                if(buffer[i] == b64_table[j]){
                    buffer[i] = j;
                    break;
                }
                //do not want to process padding
                if(buffer[i] == 0x3D){
                    buffer[i] = '=';
                    padding++;
                    break;
                }

            }
        }
        //perform decryption, base64 characters converted to 8 bit characters.
        dec[0] = (buffer[0] << 2) + ((buffer[1] & 0b110000) >> 4);
        dec[1] = ((buffer[1] & 0b1111) << 4) + ((buffer[2] & 0b111100) >> 2);
        dec[2] = ((buffer[2] & 0b11) << 6) + buffer[3];

        if(padding == 2){
            printf("%c", dec[0]); 
            
        } else if(padding == 1){
            printf("%c", dec[0]); 
            printf("%c", dec[1]); 
            
        } else{
            for(int i = 0; i < 3; i++){
                printf("%c", dec[i]);
            }
        }
        
       
        

        
        for(int i = 0; i < 4; i++){
            letter = getc(fp);
        
            while(letter == '\n' || letter == ' '){
                letter = getc(fp);
            }

            buffer[i] = letter;
        }

    }

    fclose(fp);
    return 0;

}
