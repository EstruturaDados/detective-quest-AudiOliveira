#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo o tamanho máximo para o nome da sala para evitar overflow.
#define MAX_NOME 50

/**
 * @brief Estrutura que representa um cômodo (nó) na mansão (árvore binária).
 *
 * Contém o nome da sala e ponteiros para os caminhos (filhos) à esquerda e
 * à direita.
 */
typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * @brief Cria, de forma dinâmica, uma nova sala com o nome especificado.
 *
 * @param nome_sala O nome a ser atribuído ao novo cômodo.
 * @return Sala* Um ponteiro para a nova sala criada. Retorna NULL em caso de falha.
 */
Sala* criarSala(const char *nome_sala) {
    // 1. Aloca memória para a nova sala
    Sala *nova_sala = (Sala *)malloc(sizeof(Sala));

    // 2. Verifica se a alocação foi bem-sucedida
    if (nova_sala == NULL) {
        perror("Erro ao alocar memória para a sala.");
        return NULL;
    }

    // 3. Copia o nome para o campo 'nome' da struct (garantindo que não exceda MAX_NOME)
    strncpy(nova_sala->nome, nome_sala, MAX_NOME - 1);
    nova_sala->nome[MAX_NOME - 1] = '\0'; // Garante terminação nula

    // 4. Inicializa os ponteiros dos caminhos (esquerda e direita) como NULL
    nova_sala->esquerda = NULL;
    nova_sala->direita = NULL;

    return nova_sala;
}

/**
 * @brief Permite a navegação interativa do jogador pela mansão (árvore).
 *
 * A navegação continua até que o jogador atinja um nó-folha (sala sem caminhos).
 *
 * @param sala_atual O ponteiro para a sala onde o jogador está atualmente.
 */
void explorarSalas(Sala *sala_atual) {
    char escolha;

    // Loop de exploração contínua
    while (sala_atual != NULL) {
        printf("\n\n-- Localização Atual --\n");
        printf("Você está na sala: **%s**\n", sala_atual->nome);

        // Verifica se é um cômodo que não tem mais caminhos (nó-folha)
        if (sala_atual->esquerda == NULL && sala_atual->direita == NULL) {
            printf("\n**Este cômodo não possui mais caminhos!**\n");
            printf("A exploração da mansão termina aqui. Parabéns, detetive!\n");
            break;
        }

        // Exibe as opções de navegação
        printf("\nEscolha um caminho:\n");
        if (sala_atual->esquerda != NULL) {
            printf("  [e] Esquerda -> %s\n", sala_atual->esquerda->nome);
        }
        if (sala_atual->direita != NULL) {
            printf("  [d] Direita -> %s\n", sala_atual->direita->nome);
        }
        printf("  [s] Sair do Jogo\n");

        printf("Sua escolha (e/d/s): ");
        if (scanf(" %c", &escolha) != 1) {
            // Limpa o buffer de entrada em caso de erro.
            while (getchar() != '\n');
            escolha = ' ';
        }

        // Lógica de navegação baseada na escolha do jogador
        switch (escolha) {
            case 'e':
            case 'E':
                if (sala_atual->esquerda != NULL) {
                    sala_atual = sala_atual->esquerda;
                } else {
                    printf("Caminho não disponível. Tente novamente.\n");
                }
                break;
            case 'd':
            case 'D':
                if (sala_atual->direita != NULL) {
                    sala_atual = sala_atual->direita;
                } else {
                    printf("Caminho não disponível. Tente novamente.\n");
                }
                break;
            case 's':
            case 'S':
                printf("\nVocê decidiu sair da mansão. Até a próxima!\n");
                return; // Encerra a função e, consequentemente, o jogo.
            default:
                printf("Escolha inválida. Use 'e' (esquerda), 'd' (direita) ou 's' (sair).\n");
                break;
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para toda a árvore.
 *
 * É importante para evitar vazamento de memória.
 *
 * @param raiz O nó raiz da subárvore a ser liberada.
 */
void liberarMansao(Sala *raiz) {
    if (raiz == NULL) {
        return;
    }

    // Libera a subárvore esquerda
    liberarMansao(raiz->esquerda);

    // Libera a subárvore direita
    liberarMansao(raiz->direita);

    // Libera o nó atual
    free(raiz);
}


/**
 * @brief Função principal que monta o mapa inicial e dá início à exploração.
 *
 * A estrutura da árvore é montada manualmente aqui.
 */
int main() {
    printf("====================================================\n");
    printf("        Bem-vindo ao Detective Quest: A Mansão\n");
    printf("====================================================\n");
    printf("Você é o desenvolvedor(a) técnico(a) da Enigma Studios.\n");
    printf("O mapa da mansão foi montado. Inicie a exploração!\n");

    // 1. Montagem da Árvore Binária (Mapa da Mansão)
    
    // Nível 0: Raiz (Hall de Entrada)
    Sala *hall_entrada = criarSala("Hall de Entrada");

    // Verifica se a alocação do nó raiz falhou
    if (hall_entrada == NULL) {
        return 1;
    }

    // Nível 1: Filhos do Hall de Entrada
    hall_entrada->esquerda = criarSala("Sala de Estar");
    hall_entrada->direita = criarSala("Biblioteca");

    // Nível 2: Filhos da Sala de Estar
    if (hall_entrada->esquerda != NULL) {
        hall_entrada->esquerda->esquerda = criarSala("Cozinha"); // Nó-Folha 1
        hall_entrada->esquerda->direita = criarSala("Quarto Principal");
    }

    // Nível 2: Filhos da Biblioteca
    if (hall_entrada->direita != NULL) {
        hall_entrada->direita->esquerda = criarSala("Jardim de Inverno"); // Nó-Folha 2
        hall_entrada->direita->direita = criarSala("Escritório");
    }

    // Nível 3: Filhos do Quarto Principal
    if (hall_entrada->esquerda != NULL && hall_entrada->esquerda->direita != NULL) {
        hall_entrada->esquerda->direita->esquerda = criarSala("Banheiro (Secreta)"); // Nó-Folha 3
        // hall_entrada->esquerda->direita->direita permanece NULL (Nó-Folha 4)
    }
    
    // Nível 3: Filhos do Escritório
    if (hall_entrada->direita != NULL && hall_entrada->direita->direita != NULL) {
        hall_entrada->direita->direita->esquerda = criarSala("Porão (Fim do Jogo)"); // Nó-Folha 5
        hall_entrada->direita->direita->direita = criarSala("Sótão"); // Nó-Folha 6
    }
    
    // A estrutura final da árvore é:
    // Hall_Entrada (R)
    // ├── Sala_de_Estar (L)
    // │   ├── Cozinha (L-L) -> FOLHA
    // │   └── Quarto_Principal (L-R)
    // │       ├── Banheiro (L-R-L) -> FOLHA
    // │       └── NULL (L-R-R) -> FOLHA
    // └── Biblioteca (R)
    //     ├── Jardim_de_Inverno (R-L) -> FOLHA
    //     └── Escritório (R-R)
    //         ├── Porão (R-R-L) -> FOLHA
    //         └── Sótão (R-R-R) -> FOLHA

    // 2. Início da exploração
    explorarSalas(hall_entrada);

    // 3. Liberação de memória
    printf("\nLimpando a memória da mansão...\n");
    liberarMansao(hall_entrada);

    return 0;
}