//Exercícios em c de If e Else
#include <stdio.h>

int main() {
    int num;
    printf("Digite um número inteiro: ");
    scanf("%d", &num);

    //Verificar se o número está fora do intervalo entre 10 e 20
    if (!(num >=10 && num <=20)) {
        printf("O número %d está fora do intervalo entre 10 e 20.\n",num);
    } else {
        printf("O número %d está dentro do intervalo entre 10 e 20.\n",num);
    }

    return 0;
}
