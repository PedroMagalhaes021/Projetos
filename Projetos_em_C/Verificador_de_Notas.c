//Exercícios de Verdadeiro ou Falso com números inteiros usando if e else 
#include <stdio.h>
int main() {
    int nota;
    printf("Digite a nota do aluno: ");
    scanf("%d", &nota);

    if (nota >= 6) {
        printf("Passou de ano!\n");
    } else if (nota < 6 && nota > 0) {
        printf("Reprovado\n");
    } else {
        printf("Nota inválida\n");
    }

    return 0;
}
