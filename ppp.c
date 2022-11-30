#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NTHREADS 5
#define BUFFER_SIZE 100000

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
int palindrome_size = 19;
int n_threads = NTHREADS;

typedef struct s_param {
    unsigned char buffer[BUFFER_SIZE];
    int size;
} s_param;

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
    s_param *param  = (s_param*) arg;
    char digits[palindrome_size+2];
    memset(digits, '\0', sizeof(digits));
    //printf("%s\n", (*param).buffer);
    

    for( int i = 0 ; i <= (*param).size - palindrome_size; i++ ){
        strncpy( digits, (*param).buffer+i, palindrome_size );
        //printf("%s ",digits);
        if ( is_odd_palindrome(digits) ){
            if ( is_prime(digits) ){
                printf("\nEncontrado: %s\n", digits);
                //return i+1; // retorna a posição do palindromo + 1
                pthread_exit((void *) i+1); 
            }
        }
    }

    free (arg);
    pthread_exit(NULL);
    //return 0;
}

int main(int argc, char*argv[]){
    FILE *source;
    //int n; //quantidade de elementos lidos
    unsigned long long count = 0; //quantidade de elementos lidos
    //int pos; //posição do palindromo achado no buffer
    int retorno;
    //pthread_t *tid; //identificadores das threads no sistema
    pthread_t threads[NTHREADS];

    //double tempo_1, tempo_2; //variáveis para medida de tempo

    s_param * t_param;
    
    /*for( int i=0; i<n_threads; i++) { // inicializa a struct de cada thread
        memset((*t_param[i]).buffer, '\0', sizeof((*t_param[i]).buffer));
        (*t_param[i]).size = 0;
    }*/
    int t = 0; // thread 

    //GET_TIME(tempo_1);
    if (argc < 2){
      fprintf(stderr, "Digite: %s <arquivos entrada em ordem>\n", argv[0]);
      return 1;
    }

    //unsigned char buffer[buffer_size];
    unsigned char prior_digits[palindrome_size+1];
    memset(prior_digits, '\0', sizeof(prior_digits));

    /*tid = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc tid\n");
        return 2;
    }*/

    for ( int i = 1 ; i < argc ; i++ ){ //passa por todos os arquivos

        source = fopen(argv[i], "rb");
    
        if (source)
        {
            //I primeiro loop inserindo os digitos salvos do ultimo arquivo

            /*t_param = malloc(sizeof(s_param));
            if(t_param==NULL) {
              fprintf(stderr, "ERRO--malloc PARAM\n");
              return 2;
            }
            (*t_param).size = fread( (*t_param).buffer , 1, buffer_size - palindrome_size + 1, source );
            strcpy( (*t_param).buffer , prior_digits);

            //pos = search_prime_palindrome( t_param[t] );
            if( pthread_create(&threads[t] , NULL, search_prime_palindrome, (void*) t_param) ){
              fprintf(stderr, "ERRO--pthread_create\n");
              return 3;
            }

            t++;
            if( t == n_threads ){
              for(int j=0; j<n_threads; j++) {
                if(pthread_join(threads[j], (void**) &retorno)){
                  fprintf(stderr, "ERRO--pthread_create1\n");
                  return 3;
                }

                if ( (int)retorno ){
                  printf("Posicao %llu\n", count + retorno - 1);
                  goto end_program;
                }
              }
              t=0;
            } 
           
            count += (*t_param).size - strlen(prior_digits);
            printf(".");
            //printf("%d =  %s \n",t_param[t].size, t_param[t].buffer);
            memset(prior_digits, '\0', sizeof(prior_digits));*/


            while (!feof(source))
            {
                fseek( source, (palindrome_size - 1) * (-1), SEEK_CUR ); 
                t_param = malloc(sizeof(s_param));
                if(t_param==NULL) {
                  fprintf(stderr, "ERRO--malloc PARAM\n");
                  return 2;
                }

                (*t_param).size  = fread( (*t_param).buffer, 1, buffer_size, source );
                //pos = search_prime_palindrome( t_param[t] );
                if( pthread_create(&threads[t] , NULL, search_prime_palindrome, (void*) t_param) ){
                  fprintf(stderr, "ERRO--pthread_create\n");
                  return 3;
                }

                t++;
                if( t == n_threads ){
                  for(int j=0; j<n_threads; j++) {
                    if(pthread_join(threads[j], (void**) &retorno)){
                      fprintf(stderr, "ERRO--pthread_join");
                      return 3;
                    }

                    if ( retorno ){
                      printf("Posicao: %llu\n", count + retorno - 1 - palindrome_size + 1);
                      goto end_program;
                    }
                  }
                  t=0;
                }
                

                count += (*t_param).size  - palindrome_size + 1;
                //printf("%d =  %s \n",t_param[t].size, t_param[t].buffer);
                //sleep(1);
                //printf(".");
            }

            //guarda os ultimos digitos para o buffer do proximo arquivo
            /*fseek( source, (palindrome_size - 1) * (-1), SEEK_END );
            fread( prior_digits, 1, palindrome_size - 1, source );*/
        }

        fclose(source);
    }
    end_program:
    //GET_TIME(tempo_2);
    //printf("Tempo: %e segundos\n", tempo_2-tempo_1);

    return 0;
}