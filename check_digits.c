#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int palindrome_size;

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

int main(void) {
    char digitos[50];
    
    scanf("%d", &palindrome_size);
    scanf("%s", &digitos);

    if ( is_odd_palindrome(digitos) ){
        printf("Palindromo Impar\n");
        if ( is_prime(digitos) )
            printf("Palindromo Primo\n");
    }
    else
        printf("Falso\n");


    return 0;
}