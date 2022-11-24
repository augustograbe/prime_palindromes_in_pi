#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

int BUFFER_SIZE = 1000000;
int palindrome_size = 17;

int is_odd_palindrome( char digits[] ){
    int left_check = 0;
    int right_check = palindrome_size - 1;
    int middle = right_check / 2;

    if ( (int)digits[ right_check ] % 2 == 0 ) return 0; //Se for par retorna falso

    while ( right_check > middle ){
        if ( digits[ left_check++ ] != digits[ right_check-- ] ) return 0; //Se não for palindromo retorna falso
    }

    return 1;
}

int is_prime( char str[] ){
    int digits = atoi(str); // Converte string para inteiro
    
    if ( digits % 3 == 0) return 0;
    
    int range = sqrt(digits);

    for ( int i = 5 ; i <= range ; i += 6 ){
        if ( digits % i == 0 || digits % (i + 2) == 0) return 0; // Se for divisivel retorna falso
    }

    return 1;
}

int search_prime_palindrome( char str[] ){
    char digits[palindrome_size+2];
    memset(digits, '\0', sizeof(digits));

    for( int i = 0 ; i <= BUFFER_SIZE ; i++ ){
        strncpy( digits, str+i, palindrome_size );
        if ( is_odd_palindrome(digits) ){
            if ( is_prime(digits) ){
                printf("%s ", digits);
                return i+1; // retorna a posição do palindromo + 1
            }
        }
    }

    return 0;
}

int main(int argc, char*argv[]){
    FILE *source;
    int n;
    unsigned long long count = 0; //quantidade de elementos lidos
    int pos; //posição do palindromo achado no buffer

    if (argc < 2){
      fprintf(stderr, "Digite: %s <arquivos entrada em ordem>\n", argv[0]);
      return 1;
    }

    unsigned char buffer[BUFFER_SIZE];
    unsigned char prior_digits[palindrome_size+1];
    memset(prior_digits, '\0', sizeof(prior_digits));
    
    for ( int i = 1 ; i < argc ; i++ ){

        source = fopen(argv[i], "rb");

        if (source)
        {
            //I primeiro loop inserindo os digitos salvos do ultimo arquivo
            n = fread( buffer, 1, BUFFER_SIZE - palindrome_size - 1, source );
            strcpy( buffer , prior_digits);
            pos = search_prime_palindrome(buffer);
            if ( pos ){
                    printf("Posicao %d", count + pos - 1);
                    break;
            }
            count += n;
            printf(".");
            memset(prior_digits, '\0', sizeof(prior_digits));

            while (!feof(source))
            {
                fseek( source, (palindrome_size - 1) * (-1), SEEK_CUR ); 
                n = fread( buffer, 1, BUFFER_SIZE, source );
                pos = search_prime_palindrome(buffer);
                if ( pos ){
                    printf("Posicao %d", count + pos - 1);
                    break;
                }
                count += n;
                printf(".");
            }

            //guarda os ultimos digitos para o buffer do proximo arquivo
            fseek( source, (palindrome_size - 1) * (-1), SEEK_END );
            fread( prior_digits, 1, palindrome_size - 1, source );
        }

        fclose(source);
    }

    return 0;
}