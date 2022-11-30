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

/* Variaveis globais */

int buffer_size = BUFFER_SIZE;
int palindrome_size;
int n_threads = NTHREADS;

//variaveis usadas pelaa thread
unsigned char buffer[BUFFER_SIZE];
unsigned int dim;

//variaveis de retorno das threads
int position=0;
char palindrome[30];

pthread_mutex_t x_mutex;


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
                if( position == 0 || i < position ){
                    pthread_mutex_lock(&x_mutex);
                    position = i;
                    strncpy( palindrome, digits, palindrome_size );
                    pthread_mutex_unlock(&x_mutex);
                } 
                pthread_exit(NULL); 
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char*argv[]){
    FILE *source;
    unsigned long long count = 0; //quantidade de elementos lidos
    pthread_t threads[NTHREADS];

    pthread_mutex_init(&x_mutex, NULL);

    //double tempo_1, tempo_2; //variáveis para medida de tempo

    //GET_TIME(tempo_1);
    if (argc < 2){
      fprintf(stderr, "Digite: %s <tamanho do palindromo> <arquivos entrada em ordem>\n", argv[0]);
      return 1;
    }

    palindrome_size = atoi(argv[1]);
    
    for ( int i = 2 ; i < argc ; i++ ){

        source = fopen(argv[i], "rb");

        if (source)
        {
            while (!feof(source)) //enquanto estiver digitos para ser lido cria um buffer com os digitos
            {
                fseek( source, (palindrome_size - 1) * (-1), SEEK_CUR ); 
                dim = fread( buffer, 1, BUFFER_SIZE, source );
                for(int j=0; j < n_threads; j++) { //separa a busca no buffer entre as threads
                    if(pthread_create(&threads[j], NULL, search_prime_palindrome, (void*) j)){
                        fprintf(stderr, "ERRO--pthread_create\n");
                        return 3;
                    }
                }
                for(int j=0; j < n_threads; j++) { //espera as thread completarem
                    if(pthread_join(threads[j], NULL)){
                        fprintf(stderr, "ERRO--pthread_join");
                        return 3;
                    }
                }
                if ( position ){ //verifica se alguma thread obteve resultado
                    printf("Palindromo: %s\n", palindrome);
                    printf("Posicao: %llu\n", count + position);
                    goto end_program;
                }
                     
                count += dim - palindrome_size + 1; //

                //printf(".");
            }
        }

        fclose(source);
    }
    end_program:
    //GET_TIME(tempo_2);
    //printf("Tempo: %e segundos\n", tempo_2-tempo_1);

    return 0;
}