#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1000000

/*
#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>
#define BILLION 1000000000L

#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif
*/

int buffer_size = BUFFER_SIZE;
int palindrome_size = 5;

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

int search_prime_palindrome( char str[], int n ){
    char digits[palindrome_size+2];
    memset(digits, '\0', sizeof(digits));
    //printf("%s\n ",str);
    for( int i = 0 ; i <= n - palindrome_size; i++ ){
        strncpy( digits, str+i, palindrome_size );
        //printf("%s ",digits);
        if ( is_odd_palindrome(digits) ){
            if ( is_prime(digits) ){
                printf("\nEncontrado: %s\n", digits);
                return i+1; // retorna a posição do palindromo + 1
            }
        }
    }

    return 0;
}

int main(int argc, char*argv[]){
    FILE *source;
    int n; //quantidade de elementos lidos
    unsigned long long count = 0; //quantidade de elementos lidos
    int pos; //posição do palindromo achado no buffer

    //double tempo_1, tempo_2; //variáveis para medida de tempo

    //GET_TIME(tempo_1);
    if (argc < 3){
      fprintf(stderr, "Digite: %s <tamanho do palindromo> <arquivos entrada em ordem>\n", argv[0]);
      return 1;
    }

    unsigned char buffer[BUFFER_SIZE];
    memset(buffer, '\0', sizeof(buffer));
    unsigned char prior_digits[palindrome_size+1];
    memset(prior_digits, '\0', sizeof(prior_digits));

    palindrome_size = atoi(argv[1]);
    
    for ( int i = 2 ; i < argc ; i++ ){

        source = fopen(argv[i], "rb");

        if (source)
        {
            //I primeiro loop inserindo os digitos salvos do ultimo arquivo
            n = fread( buffer, 1, buffer_size - palindrome_size + 1, source );
            strcpy( buffer , prior_digits);
            pos = search_prime_palindrome( buffer, n );
            if ( pos ){
                    printf("Posicao %llu\n", count + pos - 1);
                    goto end_program;
            }
            count += n - strlen(prior_digits);
            printf(".");
            memset(prior_digits, '\0', sizeof(prior_digits));

            while (!feof(source))
            {
                fseek( source, (palindrome_size - 1) * (-1), SEEK_CUR ); 
                n = fread( buffer, 1, buffer_size, source );
                pos = search_prime_palindrome( buffer, n );
                if ( pos ){
                    printf("Posicao: %llu\n", count + pos - 1 - palindrome_size + 1);
                    goto end_program;
                }
                count += n - palindrome_size + 1;
                //printf("%d",count);
                //sleep(1);
                printf(".");
            }

            //guarda os ultimos digitos para o buffer do proximo arquivo
            fseek( source, (palindrome_size - 1) * (-1), SEEK_END );
            fread( prior_digits, 1, palindrome_size - 1, source );
        }

        fclose(source);
    }
    end_program:
    //GET_TIME(tempo_2);
    //printf("Tempo: %e segundos\n", tempo_2-tempo_1);

    return 0;
}