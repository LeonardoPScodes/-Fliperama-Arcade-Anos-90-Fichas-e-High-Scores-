#include <stdio.h>

// 1. Leonardo Portes Silveira: Jogadores Cadastrados
struct Jogador {
    int idJogador;
    char nome[50];
    char apelido[30];
    int fichasDisponiveis;
    int ativo;
};
struct Jogador listaJogadores[50];
int totalJogadores = 0;

// 2. Guilherme Barbosa Ferreira Chagas: Catálogo de Máquinas de Arcade (Gabinetes)
struct MaquinaArcade {
    int idMaquina;
    char nomeJogo[40];
    char genero[30];
    float precoFicha;
    int emOperacao;
};
struct MaquinaArcade catalogoMaquinas[50];
int totalMaquinas = 0;

// 3. William dos Santos Oliveira: Caixa e Venda de Fichas (Créditos)
struct VendaFichas {
    int idTransacao;
    int idJogadorComprador;
    int quantidadeFichas;
    float valorTotalPago;
    char dataTransacao[15];
};
struct VendaFichas historicoFichas[50];
int totalVendasFichas = 0;

// 4. Hyan Nascimento: High Scores e Relatórios de Desempenho
struct HighScore {
    int idRegistro;
    int idJogador;
    int idMaquina;
    int pontuacaoMaxima;
    char dataRegistro[15]; 
};
struct HighScore rankingHighScores[50];
int totalHighScores = 0;


// ============================================================================
// FUNÇÕES DE INTERFACE E MÓDULOS DE CADA INTEGRANTE
// ============================================================================

// Cabeçalho Visual Retro
void exibirCabecalho() {
    printf("\n======================================================\n");
    printf("                    ARCADE 1990     \n");
    printf("======================================================\n");
}

// ----------------------------------------------------------------------------
// GESTÃO DE JOGADORES
// Leonardo Portes Silveira
// ----------------------------------------------------------------------------
void moduloJogadores() {
    int opcao;
    do {
        limparTela();
        printf("\n--- [GESTAO DE JOGADORES (Leonardo Portes Silveira)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar Jogadores e Saldo de Fichas\n");
        printf("3. Atualizar Dados do Jogador\n");
        printf("4. Remover / Desativar Jogador\n");
        printf("5. Buscar Jogador por Nome / Apelido\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInteiro("Escolha uma acao: ");
        limparTela();

        switch (opcao) {
            case 1:
                cadastrarJogador();
                break;
            case 2:
                listarJogadores();
                break;
            case 3:
                atualizarJogador();
                break;
            case 4:
                removerJogador();
                break;
            case 5:
                buscarJogadorPorNome();
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
        }

        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0);
}

// ----------------------------------------------------------------------------
// CATÁLOGO DE MÁQUINAS DE ARCADE
// Guilherme Barbosa Ferreira Chagas
// ----------------------------------------------------------------------------
void moduloMaquinas() {
    int opcao;
    do {
        printf("\n--- [CATALOGO DE MAQUINAS (Guilherme Barbosa Ferreira Chagas)] ---\n");
        printf("1. Cadastrar Nova Maquina / Jogo\n");
        printf("2. Listar Gabinetes Disponiveis\n");
        printf("3. Alterar Status de Manutencao da Maquina\n");
        printf("4. Remover Maquina Definitivamente\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n[Em construcao] Cadastrando novo gabinete (Nome, Genero, Preco)...\n");
                break;
            case 2:
                printf("\n[Em construcao] Exibindo maquinas prontas para jogar e em manutencao...\n");
                break;
            case 3:
                printf("\n[Em construcao] Alternando status de operacao da maquina...\n");
                break;
            case 4:
                printf("\n[Em construcao] Removendo maquina obsoleta...\n");
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

// ----------------------------------------------------------------------------
// CAIXA E VENDA DE FICHAS (CRÉDITOS)
// William dos Santos Oliveira
// ----------------------------------------------------------------------------
void moduloFichas() {
    int opcao;
    do {
        printf("\n--- [CAIXA E VENDA DE FICHAS (William dos Santos Oliveira)] ---\n");
        printf("1. Vender Fichas para um Jogador\n");
        printf("2. Consultar Historico de Vendas de Fichas\n");
        printf("3. Estornar Venda de Fichas\n");
        printf("4. [RELATORIO] Faturamento Total do Caixa com Fichas\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n[Em construcao] Registrando venda de fichas vinculada ao Jogador...\n");
                break;
            case 2:
                printf("\n[Em construcao] Exibindo historico de transacoes de fichas...\n");
                break;
            case 3:
                printf("\n[Em construcao] Realizando estorno de fichas por ID de transacao...\n");
                break;
            case 4:
                printf("\n[Em construcao] Calculando faturamento total obtido...\n");
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);
}

//---
// Highscore
// Hyan Castro Nascimento
void moduloHighScores() {
    int opcao;
    do {
        printf("\n--- Relatorios/Highscore ---\n");
        printf("1. Adicionar pontuação (jogador + maquina)\n");
        printf("2. Exibir Tabela Geral de High Scores\n");
        printf("3. [RELATORIO] Recordes\n");
        printf("4. [RELATORIO] Jogos mais jogados\n");
        printf("5. [RELATORIO] Historico de Records por Jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma ação: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n[Em construcao]\n");
                break;
            case 2:
                printf("\n[Em construcao]\n");
                break;
            case 3:
                printf("\n[Em construcao]\n");
                break;
            case 4:
                printf("\n[Em construcao]\n");
                break;
            case 5:
                printf("\n[Em construcao]\n");
                break;
            case 0:
                printf("\nRetornando\n");
                break;
            default:
                printf("\nErro. Tente denovo!\n");
        }
    } while (opcao != 0);
}


// ============================================================================
// FUNÇÃO PRINCIPAL (MAIN)
// ============================================================================
int main() {
    int opcaoPrincipal;

    do {
        exibirCabecalho();
        printf("SELECIONE O MODULO DO FLIPERAMA:\n");
        printf("1. GESTAO DE JOGADORES (Leonardo Portes Silveira)\n");
        printf("2. CATALOGO DE MAQUINAS (Guilherme Barbosa Ferreira Chagas)\n");
        printf("3. CAIXA E VENDA DE FICHAS (William dos Santos Oliveira)\n");
        printf("4. HIGH SCORES E RELATORIOS (Hyan Nascimento)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcaoPrincipal);

        switch (opcaoPrincipal) {
            case 1:
                moduloJogadores();
                break;
            case 2:
                moduloMaquinas();
                break;
            case 3:
                moduloFichas();
                break;
            case 4:
                moduloHighScores();
                break;
            case 0:
                printf("\nDesligando o Arcade... Obrigado por jogar, Game Over!\n");
                break;
            default:
                printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 4.\n");
        }
    } while (opcaoPrincipal != 0);

    return 0;
}
