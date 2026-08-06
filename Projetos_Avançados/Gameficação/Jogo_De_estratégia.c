// Criação do jogo de estratégia (estilo War) em C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERR 10
#define MAX_PLAYERS 4
#define NAME_LEN 32
#define MIN_REINFORCE 3

typedef struct {
    int owner;      // 0 = neutro, 1..n = jogadores
    int armies;
    char name[32];
} Territory;

typedef struct {
    char name[NAME_LEN];
    int alive;
} Player;

Territory map[MAX_TERR];
int adj[MAX_TERR][MAX_TERR]; // 1 se adjacente
Player players[MAX_PLAYERS+1];
int num_players;

// util
void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int read_int() {
    int v;
    while (scanf("%d", &v) != 1) {
        clear_input();
        printf("Entrada inválida. Tente novamente: ");
    }
    clear_input();
    return v;
}

void press_enter() {
    printf("Pressione Enter para continuar...");
    getchar();
}

// mapa inicial
void init_map() {
    // nomes dos territórios
    const char *names[MAX_TERR] = {
        "Alaska", "Noroeste", "Groenlandia", "Ontario", "Quebec",
        "America_Est", "Brasil", "Africa", "Europa", "Asia"
    };
    for (int i = 0; i < MAX_TERR; ++i) {
        map[i].owner = 0;
        map[i].armies = 1; // começa com 1 exército para não ficar vazio
        strncpy(map[i].name, names[i], sizeof(map[i].name)-1);
        map[i].name[sizeof(map[i].name)-1] = '\0';
    }
    // matriz de adjacência (simétrica)
    memset(adj, 0, sizeof(adj));
    // conexões exemplo (simples)
    int edges[][2] = {
        {0,1},{0,2}, {1,2},{1,3}, {2,3},{2,4},
        {3,4},{3,5}, {4,5}, {5,6}, {6,7}, {7,8},
        {8,9}, {3,8}, {5,8}, {6,9}
    };
    int ecount = sizeof(edges)/sizeof(edges[0]);
    for (int i=0;i<ecount;i++){
        int a=edges[i][0], b=edges[i][1];
        adj[a][b]=adj[b][a]=1;
    }
}

void print_map() {
    printf("\nMapa:\n");
    printf(" ID | Território        | Dono | Exércitos\n");
    printf("----+-------------------+------+---------\n");
    for (int i=0;i<MAX_TERR;i++){
        printf("%3d | %-17s |  %3d | %8d\n", i, map[i].name, map[i].owner, map[i].armies);
    }
    printf("\nAdjacências (por IDs):\n");
    for (int i=0;i<MAX_TERR;i++){
        printf("%2d (%s): ", i, map[i].name);
        for (int j=0;j<MAX_TERR;j++) if (adj[i][j]) printf("%d ", j);
        printf("\n");
    }
    printf("\n");
}

int territories_of_player(int p) {
    int c=0;
    for (int i=0;i<MAX_TERR;i++) if (map[i].owner==p) c++;
    return c;
}

int any_other_alive(int p) {
    for (int i=1;i<=num_players;i++) if (i!=p && players[i].alive) {
        if (territories_of_player(i)>0) return 1;
    }
    return 0;
}

int count_alive_players() {
    int cnt=0;
    for (int i=1;i<=num_players;i++){
        if (players[i].alive && territories_of_player(i)>0) cnt++;
    }
    return cnt;
}

// distribuição inicial simples: alterna territórios entre jogadores
void initial_deal() {
    int p=1;
    for (int i=0;i<MAX_TERR;i++){
        map[i].owner = p;
        map[i].armies = 1;
        p++;
        if (p>num_players) p=1;
    }
    // distribuir exércitos iniciais extras por jogador: cada jogador recebe 5 exércitos extras
    int extras = 5;
    for (int i=1;i<=num_players;i++){
        int placed = 0;
        while (placed < extras) {
            for (int t=0;t<MAX_TERR && placed < extras;t++){
                if (map[t].owner==i){
                    map[t].armies++;
                    placed++;
                }
            }
        }
    }
}

// ordenação decrescente de um array (usada nos dados)
void sort_desc(int arr[], int n) {
    for (int i=0;i<n-1;i++){
        for (int j=i+1;j<n;j++){
            if (arr[j]>arr[i]) {
                int tmp=arr[i]; arr[i]=arr[j]; arr[j]=tmp;
            }
        }
    }
}

// rolar dados: devolve array dos p maiores dados
void roll_dice(int n, int out[]) {
    for (int i=0;i<n;i++){
        out[i] = (rand()%6) + 1;
    }
    sort_desc(out, n);
}

// fase de reforço: retorna tropas a utilizar
int calculate_reinforcements(int player) {
    int terr = territories_of_player(player);
    int base = terr / 3;
    if (base < MIN_REINFORCE) base = MIN_REINFORCE;
    return base;
}

void reinforce_phase(int player) {
    int reinf = calculate_reinforcements(player);
    printf("\nJogador %d (%s) - Fase de Reforço. Recebe %d exércitos.\n", player, players[player].name, reinf);
    while (reinf > 0) {
        print_map();
        printf("Você tem %d exércitos para posicionar.\n", reinf);
        printf("Escolha o ID do território que você possui para colocar 1 exército: ");
        int id = read_int();
        if (id < 0 || id >= MAX_TERR) {
            printf("ID inválido.\n"); continue;
        }
        if (map[id].owner != player) {
            printf("Você não possui esse território.\n"); continue;
        }
        map[id].armies++;
        reinf--;
    }
}

// ataque entre dois territórios adjacentes
void attack_from_to(int attacker, int from, int to) {
    if (!adj[from][to]) { printf("Territórios não são adjacentes.\n"); return; }
    if (map[from].owner != attacker) { printf("Você não possui o território de origem.\n"); return; }
    if (map[to].owner == attacker) { printf("Território de destino já é seu.\n"); return; }
    if (map[from].armies < 2) { printf("Você precisa de pelo menos 2 exércitos para atacar (1 para permanecer).\n"); return; }

    while (map[from].armies >= 2 && map[to].armies > 0) {
        printf("\nAtaque: %s (ID %d, exércitos %d) -> %s (ID %d, dono %d, exércitos %d)\n",
               map[from].name, from, map[from].armies, map[to].name, to, map[to].owner, map[to].armies);

        int max_atk = map[from].armies - 1;
        if (max_atk > 3) max_atk = 3;
        printf("Quantos dados de ataque deseja usar? (1-%d) Ou 0 para parar ataque: ", max_atk);
        int atk_dice = read_int();
        if (atk_dice == 0) { printf("Ataque cancelado.\n"); break; }
        if (atk_dice < 1 || atk_dice > max_atk) { printf("Valor inválido.\n"); continue; }

        int def_max = map[to].armies;
        if (def_max > 2) def_max = 2;
        printf("Defensor usará %d dado(s).\n", def_max);

        int aroll[3] = {0,0,0}, droll[2] = {0,0};
        roll_dice(atk_dice, aroll);
        roll_dice(def_max, droll);
        printf("Dados atacante: ");
        for (int i=0;i<atk_dice;i++) printf("%d ", aroll[i]);
        printf("\nDados defensor: ");
        for (int i=0;i<def_max;i++) printf("%d ", droll[i]);
        printf("\n");

        int comparisons = atk_dice < def_max ? atk_dice : def_max;
        for (int i=0;i<comparisons;i++){
            if (aroll[i] > droll[i]) {
                // defensor perde 1
                map[to].armies--;
                printf("Comparação %d: atacante ganha — defensor perde 1 (def agora %d)\n", i+1, map[to].armies);
            } else {
                // atacante perde 1
                map[from].armies--;
                printf("Comparação %d: defensor ganha — atacante perde 1 (atk agora %d)\n", i+1, map[from].armies);
            }
        }

        // se defensor eliminado, atacante conquista
        if (map[to].armies <= 0) {
            printf("Território conquistado!\n");
            int old_owner = map[to].owner;
            map[to].owner = attacker;
            // mover exércitos obrigatórios: pelo menos atk_dice (ou quantos o atacante quiser até map[from].armies-1)
            int min_move = atk_dice;
            int max_move = map[from].armies - 1;
            if (max_move < min_move) min_move = max_move; // caso raro
            printf("Você deve mover ao menos %d exército(s) para o território conquistado e no máximo %d.\n", min_move, max_move);
            int move = 0;
            while (1) {
                printf("Quantos exércitos deseja mover? (entre %d e %d): ", min_move, max_move);
                move = read_int();
                if (move >= min_move && move <= max_move) break;
                printf("Número inválido. Tente novamente.\n");
            }
            map[from].armies -= move;
            map[to].armies = move;
            printf("Movidos %d exércitos para %s.\n", move, map[to].name);
            // se dono anterior perdeu todos territórios, desativa
            if (!any_other_alive(old_owner)) {
                players[old_owner].alive = 0;
                printf("Jogador %d (%s) foi eliminado!\n", old_owner, players[old_owner].name);
            }
            break;
        }

        // se atacante ficar sem forças de ataque
        if (map[from].armies < 2) {
            printf("Você não tem mais exércitos suficientes para continuar atacando desse território.\n");
            break;
        }

        printf("Deseja continuar atacando entre os mesmos territórios? 1=Sim 0=Não: ");
        int cont = read_int();
        if (!cont) break;
    }
}

// fase de ataque: jogador pode escolher pares para atacar repetidamente
void attack_phase(int player) {
    printf("\nJogador %d (%s) - Fase de Ataque.\n", player, players[player].name);
    while (1) {
        print_map();
        printf("Deseja atacar? 1=Sim 0=Não: ");
        int want = read_int();
        if (!want) break;
        printf("Escolha o ID do território de origem (seu): ");
        int from = read_int();
        if (from<0 || from>=MAX_TERR || map[from].owner!=player) {
            printf("Origem inválida.\n"); continue;
        }
        if (map[from].armies < 2) {
            printf("Origem com menos de 2 exércitos.\n"); continue;
        }
        printf("Escolha o ID do território alvo (inimigo): ");
        int to = read_int();
        if (to<0 || to>=MAX_TERR || map[to].owner==player) {
            printf("Alvo inválido.\n"); continue;
        }
        if (!adj[from][to]) { printf("Territórios não adjacentes.\n"); continue; }
        attack_from_to(player, from, to);
        // se acabou com todos os oponentes, pode terminar cedo
        if (count_alive_players() <= 1) break;
    }
}

// fase de movimentação: mover exércitos entre territórios próprios contiguos (1 movimento)
void move_phase(int player) {
    printf("\nJogador %d (%s) - Fase de Movimentação.\n", player, players[player].name);
    print_map();
    printf("Deseja mover exércitos entre seus territórios adjacentes? 1=Sim 0=Não: ");
    int want = read_int();
    if (!want) return;
    while (1) {
        printf("ID origem (seu): ");
        int from = read_int();
        if (from<0 || from>=MAX_TERR || map[from].owner!=player) { printf("Origem inválida.\n"); continue; }
        if (map[from].armies < 2) { printf("Origem precisa ter pelo menos 2 exércitos.\n"); continue; }
        printf("ID destino (seu, adjacente): ");
        int to = read_int();
        if (to<0 || to>=MAX_TERR || map[to].owner!=player) { printf("Destino inválido.\n"); continue; }
        if (!adj[from][to]) { printf("Não são adjacentes.\n"); continue; }
        int maxmove = map[from].armies - 1;
        printf("Quantos exércitos mover? (1-%d): ", maxmove);
        int mv = read_int();
        if (mv < 1 || mv > maxmove) { printf("Quantidade inválida.\n"); continue; }
        map[from].armies -= mv;
        map[to].armies += mv;
        printf("Movidos %d exércitos de %s para %s.\n", mv, map[from].name, map[to].name);
        break;
    }
}

// checa condição de vitória: retorna 0 se jogo continua, id do jogador vencedor senão
int check_victory() {
    int last = 0, count = 0;
    for (int p=1;p<=num_players;p++){
        if (players[p].alive && territories_of_player(p) > 0) {
            last = p; count++;
        }
    }
    if (count == 1) return last;
    return 0;
}

void setup_players() {
    printf("Número de jogadores (2-4): ");
    int n = read_int();
    while (n < 2 || n > 4) {
        printf("Valor inválido. Insira 2, 3 ou 4: ");
        n = read_int();
    }
    num_players = n;
    clear_input();
    for (int i=1;i<=num_players;i++){
        players[i].alive = 1;
        printf("Nome do jogador %d: ", i);
        if (fgets(players[i].name, sizeof(players[i].name), stdin) == NULL) {
            strncpy(players[i].name, "Jogador", sizeof(players[i].name)-1);
        } else {
            // remove newline
            size_t ln = strlen(players[i].name);
            if (ln>0 && players[i].name[ln-1]=='\n') players[i].name[ln-1]='\0';
        }
    }
}

int main() {
    srand((unsigned)time(NULL));
    printf("=== WAR (versão simplificada em C) ===\n");
    init_map();
    setup_players();
    initial_deal();

    int current = 1;
    // loop principal
    while (1) {
        if (!players[current].alive || territories_of_player(current) == 0) {
            // pular jogador eliminado
            current++;
            if (current > num_players) current = 1;
            continue;
        }

        printf("\n====================================\n");
        printf("Vez do jogador %d (%s)\n", current, players[current].name);
        printf("Territórios possuídos: %d\n", territories_of_player(current));
        press_enter();

        // Reforço
        reinforce_phase(current);

        // Ataque
        attack_phase(current);

        // Movimentação
        move_phase(current);

        // verificar vitória
        int winner = check_victory();
        if (winner) {
            printf("\n=== Jogo encerrado! Jogador %d (%s) venceu! ===\n", winner, players[winner].name);
            print_map();
            break;
        }

        // próximo jogador vivo
        current++;
        if (current > num_players) current = 1;
    }

    printf("Obrigado por jogar!\n");
    return 0;
}
