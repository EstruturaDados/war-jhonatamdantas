#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

typedef struct 
{
    char name[30];
    char color[10];
    int troops;
} dataTerritory;

// ==========================================
// PROTÓTIPOS DAS FUNÇÕES
// ==========================================
int exibirMenuPrincipal();
dataTerritory* registrarTerritorio(int* qtd_ref); // Alterado para devolver o mapa pro main!
void exibirTerritorio(dataTerritory* territorio, int QTD_TERRITORIO);
void faseAtaque(dataTerritory* territory, int QTD_TERRITORIO);
void simularAtaque(dataTerritory* atacante, dataTerritory* defensor);
void liberarMemoria(dataTerritory* territorio);

// Funções de Missão
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, dataTerritory* mapa, int tamanho);


// ==========================================
// FUNÇÃO PRINCIPAL (MAIN)
// ==========================================
int main() {
    // 1. Configuração Inicial (Setup):
    srand(time(NULL)); // Inicializa a semente de números aleatórios

    int QTD_TERRITORIO = 0;
    
    // O main agora recebe o mapa e a quantidade para poder usar no loop do jogo!
    dataTerritory* territory = registrarTerritorio(&QTD_TERRITORIO);
    if (territory == NULL) {
        return 1; // Encerra se der erro de memória
    }

    // --- SETUP DAS MISSÕES ---
    char* listaMissoes[5] = {
        "Eliminar todas as tropas da cor vermelha",
        "Eliminar todas as tropas da cor azul",
        "Acumular 20 tropas espalhadas pelo mundo",
        "Dominar o territorio Europa",
        "Conquistar 3 territorios diferentes"
    };

    // Alocação dinâmica para armazenar a missão do jogador
    char* missaoDoJogador = malloc(100 * sizeof(char));
    if (missaoDoJogador == NULL) {
        printf("Erro de memoria ao alocar a missao!\n");
        liberarMemoria(territory);
        return 1;
    }

    atribuirMissao(missaoDoJogador, listaMissoes, 5);
    printf("\n*** O JOGO VAI COMEÇAR! ***\nSua missão foi definida secretamente. Boa sorte!\n\n");
    system("pause"); // Pausa para o jogador ler antes de limpar/continuar

    // 2. Laço Principal do Jogo (Game Loop):
    int opcao;
    bool jogoRodando = true;

    do {
        printf("\n====================================\n");
        exibirTerritorio(territory, QTD_TERRITORIO);
        
        opcao = exibirMenuPrincipal();

        switch(opcao) {
            case 1:
                faseAtaque(territory, QTD_TERRITORIO);
                break;
            case 2:
                printf("\n--- SUA MISSÃO SECRETA ---\n");
                printf("%s\n", missaoDoJogador);
                printf("--------------------------\n");
                system("pause"); // Pausa para o jogador ler
                break;
            case 0:
                printf("\nSaindo do Game...\n");
                jogoRodando = false;
                break;
            default:
                printf("\nOpção Inválida!\n");
                break;
        }

        // Exibição condicional: Verifica se ganhou após fazer uma ação (se não tiver saído)
        if (opcao != 0) {
            int venceu = verificarMissao(missaoDoJogador, territory, QTD_TERRITORIO);
            if (venceu == 1) {
                printf("\n=======================================================\n");
                printf("🏆 VITÓRIA! O jogo acabou!\n");
                printf("Você cumpriu a sua missão: %s\n", missaoDoJogador);
                printf("=======================================================\n");
                jogoRodando = false; // Quebra o laço e finaliza
            }
        }

    } while (jogoRodando);

    // 3. Limpeza:
    free(missaoDoJogador); // Libera a missão alocada
    liberarMemoria(territory); // Libera o mapa

    return 0;
}


// ==========================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ==========================================

dataTerritory* registrarTerritorio(int* qtd_ref) {
    int QTD_TERRITORIO;
    printf("Quantos Territórios deseja cadastrar: ");
    scanf("%d", &QTD_TERRITORIO);
    
    *qtd_ref = QTD_TERRITORIO; // Salva a quantidade lá na variável do main

    dataTerritory *territory = malloc(QTD_TERRITORIO * sizeof(dataTerritory));
    if (territory == NULL){
        printf("Erro: Memoria insuficiente!\n");
        return NULL;
    }

    printf("\n--- Vamos Cadastrar %d territórios iniciais nesse mundo. --- \n", QTD_TERRITORIO);
    for (int i = 0; i < QTD_TERRITORIO; i++){
        printf("---- CADASTRO DO TERRITÓRIO %d ---- \n", i);
        // Uso do %[^\n] para permitir espaços no nome!
        printf("Nome do Território: "); scanf(" %[^\n]", territory[i].name);
        printf("Cor (Azul, Verde, Vermelha...): "); scanf(" %[^\n]", territory[i].color);
        printf("Tropas: "); scanf("%d", &territory[i].troops);
        printf("\n");
    }
    
    // Retorna o ponteiro para o main usar
    return territory; 
}

int exibirMenuPrincipal() {
    int op;
    printf("\nMENU DE OPÇÕES\n");
    printf("1 -> ATACAR\n");
    printf("2 -> VER MISSÃO\n");
    printf("0 -> SAIR\n");
    printf("Sua Opção: ");
    scanf("%d", &op);
    return op;
}

void exibirTerritorio(dataTerritory* territory, int QTD_TERRITORIO) {
    printf("--- Mapa Atualizado --- \n");
    for (int i = 0; i < QTD_TERRITORIO; i++){
        printf("[%d] Território: %s | Cor: %s | Tropas: %d \n", i, territory[i].name, territory[i].color, territory[i].troops);
    }
    if (QTD_TERRITORIO == 1){
        printf("AVISO: APENAS UM TERRITÓRIO CADASTRADO (Sem alvos para ataque)\n");
    }
    printf("-----------------------\n");
}

void faseAtaque(dataTerritory* territory, int QTD_TERRITORIO) {
    if (QTD_TERRITORIO <= 1) {
        printf("\nErro: Número de territórios insuficiente para guerra!\n");
        return;
    }

    int territorioAtacante, territorioDefensor;

    while(true){
        printf("\nQual território deseja usar para ataque (0 a %d ou -1 para sair): ", QTD_TERRITORIO-1);
        scanf("%d", &territorioAtacante);

        if (territorioAtacante == -1){
            printf("Voltando ao menu principal...\n");
            break;
        }

        printf("Qual território deseja atacar (0 a %d ou -1 para sair): ", QTD_TERRITORIO-1);
        scanf("%d", &territorioDefensor);

        if (territorioDefensor == -1){
            printf("Voltando ao menu principal...\n");
            break;
        }

        // Validações
        if (territorioAtacante >= QTD_TERRITORIO || territorioAtacante < 0){
            printf("-> Território atacante não cadastrado! \n");
            continue;
        }
        else if (territorioDefensor >= QTD_TERRITORIO || territorioDefensor < 0){
            printf("-> Território defensor não cadastrado! \n");
            continue;
        }
        else if (territorioAtacante == territorioDefensor){
            printf("-> Erro: Um território não pode atacar a si mesmo! \n");
            continue;
        }
        else {
            // simularAtaque(&territory[territorioAtacante], &territory[territorioDefensor]);
            printf("\n>>> SIMULANDO ATAQUE DE %s CONTRA %s <<<\n", territory[territorioAtacante].name, territory[territorioDefensor].name);
            break; // Sai do while de ataque após dar o comando
        }
    } 
}

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

int verificarMissao(char* missao, dataTerritory* mapa, int tamanho) {
    if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if ((strcmp(mapa[i].color, "Vermelha") == 0 || strcmp(mapa[i].color, "vermelha") == 0) && mapa[i].troops > 0) return 0;
        }
        return 1;
    }
    else if (strcmp(missao, "Eliminar todas as tropas da cor azul") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if ((strcmp(mapa[i].color, "Azul") == 0 || strcmp(mapa[i].color, "azul") == 0) && mapa[i].troops > 0) return 0;
        }
        return 1;
    }
    else if (strcmp(missao, "Acumular 20 tropas espalhadas pelo mundo") == 0) {
        int total = 0;
        for (int i = 0; i < tamanho; i++) total += mapa[i].troops;
        if (total >= 20) return 1;
    }
    else if (strcmp(missao, "Dominar o territorio Europa") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].name, "Europa") == 0 && mapa[i].troops >= 5) return 1;
        }
    }
    else if (strcmp(missao, "Conquistar 3 territorios diferentes") == 0) {
        int dominados = 0;
        for (int i = 0; i < tamanho; i++) {
            if (mapa[i].troops > 0) dominados++;
        }
        if (dominados >= 3) return 1;
    }
    return 0;
}

void liberarMemoria(dataTerritory* territorio){
    free(territorio);
}