#include <stdio.h>
#include <string.h>

// 1. Leonardo Portes Silveira: Jogadores Cadastrados
struct Jogador {
    int idJogador;
    char nome[50];
    char apelido[30]; // Nickname clássico (ex: "PACMAN99")
    int fichasDisponiveis;
    int ativo; // 1 = Ativo, 0 = Inativo
};
struct Jogador listaJogadores[50];
int totalJogadores = 0;

// 2. Guilherme Barbosa Ferreira Chagas: Catálogo de Máquinas de Arcade (Gabinetes)
struct MaquinaArcade {
    int idMaquina;
    char nomeJogo[40];    // Ex: Street Fighter II, Pac-Man, Metal Slug
    char genero[30];      // Ex: Luta, Labirinto, Tiro
    float precoFicha;     // Custo em créditos por partida
    int emOperacao;       // 1 = Funcionando, 0 = Em Manutenção
};
struct MaquinaArcade catalogoMaquinas[50];
int totalMaquinas = 0;

// 3. William dos Santos Oliveira: Caixa e Venda de Fichas (Créditos)
struct VendaFichas {
    int idTransacao;
    int idJogadorComprador; // Cruzamento com a struct Jogador
    int quantidadeFichas;
    float valorTotalPago;
    char dataTransacao[15]; // Ex: "28/10/2026"
};
struct VendaFichas historicoFichas[50];
int totalVendasFichas = 0;

// 4. Hyan Nascimento: High Scores e Relatórios de Desempenho
struct HighScore {
    int idRegistro;
    int idJogador;        // Cruzamento com a struct Jogador
    int idMaquina;        // Cruzamento com a struct MaquinaArcade
    int pontuacaoMaxima;  // Score alcançado
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
        printf("\n--- [GESTAO DE JOGADORES (Leonardo Portes Silveira)] ---\n");
        printf("1. Cadastrar Novo Jogador\n");
        printf("2. Listar Jogadores e Saldo de Fichas\n");
        printf("3. Atualizar Dados do Jogador\n");
        printf("4. Remover / Desativar Jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n[Em construcao] Cadastro de Jogador (ID, Nome, Apelido/Nick)...\n");
                break;
            case 2:
                printf("\n[Em construcao] Listagem geral de jogadores cadastrados...\n");
                break;
            case 3:
                printf("\n[Em construcao] Editando dados do jogador...\n");
                break;
            case 4:
                printf("\n[Em construcao] Desativando perfil de jogador...\n");
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

// ----------------------------------------------------------------------------
// HIGH SCORES E RELATÓRIOS ANALÍTICOS
// Hyan Nascimento
// ----------------------------------------------------------------------------
void moduloHighScores() {
    int opcao;
    do {
        printf("\n--- [HIGH SCORES E RELATORIOS (Hyan Nascimento)] ---\n");
        printf("1. Registrar Nova Pontuacao (Cruza Jogador + Maquina)\n");
        printf("2. Exibir Tabela Geral de High Scores\n");
        printf("3. [RELATORIO] Campeao Geral de Pontuacao (Recordista)\n");
        printf("4. [RELATORIO] Jogo Mais Jogado / Popular\n");
        printf("5. [RELATORIO] Historico de Records por Jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n[Em construcao] Inserindo score e vinculando ID Jogador + ID Jogo...\n");
                break;
            case 2:
                printf("\n[Em construcao] Exibindo Hall da Fama (High Scores)...\n");
                break;
            case 3:
                printf("\n[Em construcao] Calculando o maior recordista do arcade...\n");
                break;
            case 4:
                printf("\n[Em construcao] Analisando o gabinete com mais pontuacoes registradas...\n");
                break;
            case 5:
                printf("\n[Em construcao] Filtrando records de um jogador especifico...\n");
                break;
            case 0:
                printf("\nRetornando ao menu principal...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida!\n");
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
