# Crypto-P1

Program contains 3 functions: hexdump, base64 encode, base64 decode. 

After creating an executable (using makeFile), functions can be run using the following syntax: 
    proj1 {hexdump|enc-base64|dec-base64} [file]

Each function takes a file pointer parameter and returns an int indicating if the program was successful or not. 

hexdump prints the hex values for each character in a string. If the character value is below 32, it is represented as a ".", and if it is above 127 it is represented as a "~". 

enc-base64 prints out the base64 encryption of a file. It may add padding as needed ("=" or "==" or no padding). 

dec-base64 prints out the base64 decryption of a file. Note that this program will only function properly if it receives a valid ciphertext (proper padding, only using base64 characters). 