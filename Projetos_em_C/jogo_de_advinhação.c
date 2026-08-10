#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()

{
int numeroSecreto;
int palpite;
int tentativas = 0;
int pontos = 100;

srand(time(NULL));

numeroSecreto= rand()%100 + 1;


printf("=======================\n");
printf("    Game da Advinhação   \n");
printf("========================\n");

printf("\nestou pensando em um número de 1 a 100\n");
printf("Tente descibrir qual é!\n\n");

do {
    printf("Digite seu palpite: ");
    scanf("%d", &palpite);
    
    tentativas ++;
    
    if (palpite<numeroSecreto){
        printf("Muito baixo! Tente Novamente!\n");
        pontos -= 10;
    }
    else if(palpite>numeroSecreto) {
        printf("Muito Alto! Tente um número menor.\n");
        pontos -= 10;
    }
    else {
        printf("\n========================\n");
        printf("PARABENS VOCÊ ACERTOU !\n");
        printf("==========================\n");
        
        printf("numero secreto: %d\n",numeroSecreto);
        printf("tentativas: %d\n",tentativas);
        
        if(pontos<0) {
            pontos= 0;
        }
        printf("pontuação: %d pontos\n",pontos);
    }
    printf("\n");
    
    } while (palpite!=numeroSecreto);

return 0;
}
