#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// ============================================================================
// PARTE 1: MODELAGEM DE DADOS - TEMA FLIPERAMA / ARCADE ANOS 90
// =========================================================================

// Leonardo Portes Silveira: Jogadores Cadastrados
struct Jogador {
    int idJogador;
    char nome[50];
    char apelido[30]; // Nickname clássico (ex: "PACMAN99")
    int fichasDisponiveis;
    int ativo; // 1 = Ativo, 0 = Inativo
};
struct Jogador listaJogadores[50];
int totalJogadores = 0;

//Guilherme Barbosa Ferreira Chagas: Catálogo de Máquinas de Arcade (Gabinetes)
struct MaquinaArcade {
    int idMaquina;
    char nomeJogo[40];    // Ex: Street Fighter II, Pac-Man, Metal Slug
    char genero[30];      // Ex: Luta, Labirinto, Tiro
    float precoFicha;     // Custo em créditos por partida
    int emOperacao;       // 1 = Funcionando, 0 = Em Manutenção
};
struct MaquinaArcade catalogoMaquinas[50];
int totalMaquinas = 0;

//William dos Santos Oliveira: Caixa e Venda de Fichas (Créditos)
struct VendaFichas {
    int idTransacao;
    int idJogadorComprador; // Cruzamento com a struct Jogador
    int quantidadeFichas;
    float valorTotalPago;
    char dataTransacao[15]; // Ex: "28/10/2026"
};
struct VendaFichas historicoFichas[50];
int totalVendasFichas = 0;

//Hyan Nascimento: High Scores e Relatórios de Desempenho
struct HighScore {
    int idRegistro;
    int idJogador;        // Cruzamento com a struct Jogador
    int idMaquina;        // Cruzamento com a struct MaquinaArcade
    int pontuacaoMaxima;  // Score alcançado
    char dataRegistro[15];
};
struct HighScore rankingHighScores[50];
int totalHighScores = 0;

// Modulos do projeto (na mesma pasta do arquivo principal)
#include "Utils.h"
#include "Jogadores.h"
#include "Maquinas.h"
#include "Fichas.h"
#include "HighScores.h"

// ============================================================================
// PARTE 2: FUNÇÕES DE INTERFACE E MÓDULOS DE CADA INTEGRANTE
// ============================================================================

// Cabeçalho
void exibirCabecalho() {
    printf("\n======================================================\n");
    printf("     ARCADE 1990 - CENTRO DE DIVERSOES ELETRONICAS     \n");
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
        limparTela();
        printf("\n--- [CATALOGO DE MAQUINAS (Guilherme Barbosa Ferreira Chagas)] ---\n");
        printf("1. Cadastrar Nova Maquina / Jogo\n");
        printf("2. Listar Gabinetes Disponiveis\n");
        printf("3. Alterar Status de Manutencao da Maquina\n");
        printf("4. Remover Maquina Definitivamente\n");
        printf("5. Editar Dados da Maquina\n");
        printf("6. Buscar Maquina por Nome / Genero\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInteiro("Escolha uma acao: ");
        limparTela();

        switch (opcao) {
            case 1:
                cadastrarMaquina();
                break;
            case 2:
                listarMaquinas();
                break;
            case 3:
                alterarStatusMaquina();
                break;
            case 4:
                removerMaquina();
                break;
            case 5:
                editarMaquina();
                break;
            case 6:
                buscarMaquinaPorNome();
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
// CAIXA E VENDA DE FICHAS (CRÉDITOS)
// William dos Santos Oliveira
// ----------------------------------------------------------------------------
void moduloFichas() {
    int opcao;
    do {
        limparTela();
        printf("\n--- [CAIXA E VENDA DE FICHAS (William dos Santos Oliveira)] ---\n");
        printf("1. Vender Fichas para um Jogador\n");
        printf("2. Consultar Historico de Vendas de Fichas\n");
        printf("3. Estornar Venda de Fichas\n");
        printf("4. [RELATORIO] Faturamento Total do Caixa com Fichas\n");
        printf("5. Buscar Vendas por Jogador\n");
        printf("6. Editar Venda de Fichas\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInteiro("Escolha uma acao: ");
        limparTela();

        switch (opcao) {
            case 1:
                venderFichas();
                break;
            case 2:
                consultarHistoricoVendas();
                break;
            case 3:
                estornarVenda();
                break;
            case 4:
                faturamentoTotal();
                break;
            case 5:
                buscarVendasPorJogador();
                break;
            case 6:
                editarVenda();
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
// HIGH SCORES E RELATÓRIOS ANALÍTICOS
// Hyan Nascimento
// ----------------------------------------------------------------------------
void moduloHighScores() {
    int opcao;
    do {
        limparTela();
        printf("\n--- [HIGH SCORES E RELATORIOS (Hyan Nascimento)] ---\n");
        printf("1. Registrar Nova Pontuacao (Cruza Jogador + Maquina)\n");
        printf("2. Exibir Tabela Geral de High Scores\n");
        printf("3. [RELATORIO] Campeao Geral de Pontuacao (Recordista)\n");
        printf("4. [RELATORIO] Jogo Mais Jogado / Popular\n");
        printf("5. [RELATORIO] Historico de Records por Jogador\n");
        printf("6. Buscar Pontuacoes por Jogador / Jogo\n");
        printf("7. Editar Pontuacao Registrada\n");
        printf("8. Remover Pontuacao Registrada\n");
        printf("9. [RELATORIO] Matriz Jogador x Maquina (Melhor Pontuacao)\n");
        printf("0. Voltar ao Menu Principal\n");
        opcao = lerInteiro("Escolha uma acao: ");
        limparTela();

        switch (opcao) {
            case 1:
                registrarPontuacao();
                break;
            case 2:
                exibirHighScores();
                break;
            case 3:
                campeaoGeral();
                break;
            case 4:
                jogoMaisJogado();
                break;
            case 5:
                historicoPorJogador();
                break;
            case 6:
                buscarPontuacao();
                break;
            case 7:
                editarPontuacao();
                break;
            case 8:
                removerPontuacao();
                break;
            case 9:
                matrizJogadorMaquina();
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


// ============================================================================
// PARTE 3: FUNÇÃO PRINCIPAL (MAIN)
// ============================================================================
int main() {
    int opcaoPrincipal, c;

    // Ativa os acentos do Portugues sem alterar o ponto decimal (.) dos floats
    setlocale(LC_CTYPE, "Portuguese");

    // Carrega automaticamente os dados dos arquivos .csv ao abrir o programa
    carregarJogadoresCSV();
    carregarMaquinasCSV();
    carregarFichasCSV();
    carregarHighScoresCSV();

    // Mostra quantos registros foram lidos de cada .csv
    printf("[CSV] Jogadores: %d | Maquinas: %d | Vendas: %d | Pontuacoes: %d\n",
           totalJogadores, totalMaquinas, totalVendasFichas, totalHighScores);
    if (totalJogadores == 0 || totalMaquinas == 0) {
        printf("[ALERTA] Algum .csv nao foi lido. Confira se os 4 arquivos .csv estao na MESMA pasta do programa (.exe).\n");
    }
    printf("\nPressione ENTER para iniciar...");
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    do {
        limparTela();
        exibirCabecalho();
        printf("SELECIONE O MODULO DO FLIPERAMA:\n");
        printf("1. GESTAO DE JOGADORES (Leonardo Portes Silveira)\n");
        printf("2. CATALOGO DE MAQUINAS (Guilherme Barbosa Ferreira Chagas)\n");
        printf("3. CAIXA E VENDA DE FICHAS (William dos Santos Oliveira)\n");
        printf("4. HIGH SCORES E RELATORIOS (Hyan Nascimento)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        opcaoPrincipal = lerInteiro("Digite a opcao desejada: ");

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
                pausar();
        }
    } while (opcaoPrincipal != 0);

    return 0;
}
