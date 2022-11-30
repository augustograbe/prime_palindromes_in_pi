#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int palindrome_size = 15;
int position =  30060611;

int main(int argc, char*argv[]){
    FILE *source;
    unsigned char digits[palindrome_size+2];
    memset(digits, '\0', sizeof(digits));

    if (argc < 2){
      fprintf(stderr, "Digite: %s <arquivo de entrada>\n", argv[0]);
      return 1;
    }
    source = fopen(argv[1], "rb");                
    fseek( source, position, SEEK_SET );
    fread( digits, 1, palindrome_size, source );

    printf("%s\n", digits);

    /*fseek( source, position - palindrome_size, SEEK_SET );
    fread( digits, 1, palindrome_size, source );
    printf("antes %s\n", digits);

    fseek( source, position + palindrome_size, SEEK_SET );
    fread( digits, 1, palindrome_size, source );
    printf("depois %s\n", digits);*/

    fclose(source);
    return 0;
}