//Exercícios de Verdadeiro ou Falso com números inteiros usando if e else 
#include <stdio.h>

int main() {
    int num;
    printf("Digite um número inteiro: ");
    scanf("%d", &num);

    if (num % 2 == 0) {
        printf("O número %d é PAR\n", num);
    } else {
        printf("O número %d é ÍMPAR\n", num);
    }
    return 0;
}
