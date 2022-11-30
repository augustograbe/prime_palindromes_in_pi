#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NTHREADS 4
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
int n_threads = NTHREADS;

unsigned char buffer[BUFFER_SIZE];
unsigned int dim;

typedef struct pp{
    int position;
    char palindrome;
}pp;

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

void * search_prime_palindrome( void * arg ){
    int id = (int) arg;
    char digits[palindrome_size+2];
    memset(digits, '\0', sizeof(digits));

    for( int i = id ; i <= dim - palindrome_size; i += n_threads ){
        strncpy( digits, buffer+i, palindrome_size );
        if ( is_odd_palindrome(digits) ){
            if ( is_prime(digits) ){
                //printf("\nEncontrado: %s\n", digits);
                pp results;
                results.position = i + 1;
                results.palindrome = digits;
                pthread_exit((void *) &results); 
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char*argv[]){
    FILE *source;
    int n; //quantidade de elementos lidos
    unsigned long long count = 0; //quantidade de elementos lidos
    int pos; //posição do palindromo achado no buffer
    int retorno;
    pthread_t threads[NTHREADS];
    pp results;

    //double tempo_1, tempo_2; //variáveis para medida de tempo

    //GET_TIME(tempo_1);
    if (argc < 2){
      fprintf(stderr, "Digite: %s <arquivos entrada em ordem>\n", argv[0]);
      return 1;
    }
  
    int min=0;
    char r;

    //unsigned char buffer[BUFFER_SIZE];
    memset(buffer, '\0', sizeof(buffer));
    //unsigned char prior_digits[palindrome_size+1];
    //memset(prior_digits, '\0', sizeof(prior_digits));
    
    for ( int i = 1 ; i < argc ; i++ ){

        source = fopen(argv[i], "rb");

        if (source)
        {
            //I primeiro loop inserindo os digitos salvos do ultimo arquivo
            /*n = fread( buffer, 1, BUFFER_SIZE - palindrome_size + 1, source );
            strcpy( buffer , prior_digits);
            pos = search_prime_palindrome( buffer, n );
            if ( pos ){
                    printf("Posicao %llu\n", count + pos - 1);
                    goto end_program;
            }
            count += n - strlen(prior_digits);
            printf(".");
            memset(prior_digits, '\0', sizeof(prior_digits));*/

            while (!feof(source))
            {
                fseek( source, (palindrome_size - 1) * (-1), SEEK_CUR ); 
                dim = fread( buffer, 1, BUFFER_SIZE, source );
                //pos = search_prime_palindrome( buffer, n );
                for(int j=0; j < n_threads; j++) {
                    if(pthread_create(&threads[j], NULL, search_prime_palindrome, (void*) j)){
                        fprintf(stderr, "ERRO--pthread_create\n");
                        return 3;
                    }
                }
                for(int j=0; j < n_threads; j++) {
                    if(pthread_join(threads[j], (void**) &results)){
                        fprintf(stderr, "ERRO--pthread_join");
                        return 3;
                    }
                    if( min==0)
                        min = results.position;
                    if( results.position != 0 & results.position < min){
                        min = results.position;
                        r = results.palindrome;
                    }
                }
                if ( min ){
                    printf("Palindromo: %s\n", r);
                    printf("Posicao: %llu\n", count + min - 1 - palindrome_size + 1);
                    goto end_program;
                }
                     
                count += dim - palindrome_size + 1;

                //printf(".");
            }

            //guarda os ultimos digitos para o buffer do proximo arquivo
            //fseek( source, (palindrome_size - 1) * (-1), SEEK_END );
            //fread( prior_digits, 1, palindrome_size - 1, source );
        }

        fclose(source);
    }
    end_program:
    //GET_TIME(tempo_2);
    //printf("Tempo: %e segundos\n", tempo_2-tempo_1);

    return 0;
}