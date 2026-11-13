#ifndef JOGADORES_H
#define JOGADORES_H

// ============================================================================
// FUNCOES DE JOGADORES
// ============================================================================

// ----------------------------------------------------------------------------
// PERSISTENCIA: grava (ou regrava) todo o vetor de jogadores em jogadores.csv
// ----------------------------------------------------------------------------
void salvarJogadoresCSV() {
    FILE *arq;
    int i;

    arq = fopen("jogadores.csv", "w"); // modo "w" zera o arquivo e regrava o vetor atualizado
    if (arq == NULL) {
        printf("\nOps! Nao consegui salvar o arquivo jogadores.csv.\n");
        printf("Dica: se ele estiver aberto no Excel ou no Bloco de Notas, feche e tente de novo.\n");
        return;
    }

    for (i = 0; i < totalJogadores; i++) {
        fprintf(arq, "%d;%s;%s;%d;%d\n",
                listaJogadores[i].idJogador,
                listaJogadores[i].nome,
                listaJogadores[i].apelido,
                listaJogadores[i].fichasDisponiveis,
                listaJogadores[i].ativo);
    }

    fclose(arq);
}

// ----------------------------------------------------------------------------
// PERSISTENCIA: carrega o jogadores.csv para o vetor ao abrir o programa
// ----------------------------------------------------------------------------
void carregarJogadoresCSV() {
    FILE *arq;
    char linha[300];
    struct Jogador j;

    arq = fopen("jogadores.csv", "r");
    if (arq == NULL) {
        return; // se o arquivo ainda nao existe, comeca com 0 registros
    }

    totalJogadores = 0;
    while (totalJogadores < 50 && lerLinhaCSV(arq, linha, 300)) {
        // linhas fora do padrao (vazias, cabecalho, incompletas) sao ignoradas
        if (sscanf(linha, "%d;%49[^;];%29[^;];%d;%d",
                   &j.idJogador, j.nome, j.apelido,
                   &j.fichasDisponiveis, &j.ativo) == 5) {
            maiusculas(j.apelido);
            listaJogadores[totalJogadores] = j;
            totalJogadores++;
        }
    }

    fclose(arq);
}

// ----------------------------------------------------------------------------
// 1. CADASTRAR JOGADOR
// ----------------------------------------------------------------------------
void cadastrarJogador() {
    struct Jogador novo;
    int i;

    printf("\n=== Vamos cadastrar um novo jogador! ===\n");

    if (totalJogadores >= 50) {
        printf("Poxa, o limite de 50 jogadores ja foi atingido. Nao da para cadastrar mais ninguem por enquanto.\n");
        return;
    }

    // ID automatico
    novo.idJogador = 1;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador >= novo.idJogador) {
            novo.idJogador = listaJogadores[i].idJogador + 1;
        }
    }

    printf("Qual e o nome completo do jogador? ");
    scanf(" %49[^\n]", novo.nome);
    tirarPontoVirgula(novo.nome);

    printf("E qual apelido (nick) ele vai usar? (ex: PACMAN99): ");
    scanf(" %29[^\n]", novo.apelido);
    tirarPontoVirgula(novo.apelido);
    maiusculas(novo.apelido);

    // Verifica se ja existe
    for (i = 0; i < totalJogadores; i++) {
        if (textosIguais(listaJogadores[i].apelido, novo.apelido) == 1) {
            printf("\nEita! Esse apelido ja pertence a outro jogador. Tente cadastrar de novo com um nick diferente.\n");
            return;
        }
    }

    novo.fichasDisponiveis = 0;
    novo.ativo = 1;

    listaJogadores[totalJogadores] = novo;
    totalJogadores++;
    salvarJogadoresCSV();

    printf("\nPronto! %s foi cadastrado com sucesso. (ID: %d)\n", novo.apelido, novo.idJogador);
    printf("Tudo ficou guardadinho em 'jogadores.csv'.\n");
}

// ----------------------------------------------------------------------------
// 2. LISTAR JOGADORES
// ----------------------------------------------------------------------------
void listarJogadores() {
    int i;

    printf("\n=== Jogadores cadastrados ===\n");

    if (totalJogadores == 0) {
        printf("Ainda nao ha ninguem por aqui. Que tal cadastrar o primeiro jogador?\n");
        return;
    }

    printf("%-4s | %-25s | %-15s | %-7s | %-8s\n",
           "ID", "NOME", "APELIDO", "FICHAS", "STATUS");
    printf("-----+---------------------------+-----------------+---------+---------\n");

    for (i = 0; i < totalJogadores; i++) {
        printf("%-4d | %-25.25s | %-15.15s | %-7d | ",
               listaJogadores[i].idJogador,
               listaJogadores[i].nome,
               listaJogadores[i].apelido,
               listaJogadores[i].fichasDisponiveis);
        if (listaJogadores[i].ativo == 1) {
            printf("Ativo\n");
        } else {
            printf("Inativo\n");
        }
    }

    if (totalJogadores == 1) {
        printf("\nTemos 1 jogador cadastrado.\n");
    } else {
        printf("\nTemos %d jogadores cadastrados.\n", totalJogadores);
    }
}

// ----------------------------------------------------------------------------
// 3. ATUALIZAR DADOS DO JOGADOR
// ----------------------------------------------------------------------------
void atualizarJogador() {
    int id, pos, opcao, i;
    char novoApelido[30];

    printf("\n=== Atualizar dados de um jogador ===\n");

    if (totalJogadores == 0) {
        printf("Ainda nao ha ninguem cadastrado para atualizar.\n");
        return;
    }

    id = lerInteiro("Qual e o ID do jogador? ");
    pos = indiceJogadorPorId(id);

    if (pos == -1) {
        printf("Hmm, nao encontrei nenhum jogador com o ID %d. Confira o numero e tente novamente.\n", id);
        return;
    }

    printf("\nVoce escolheu: %s (%s)\n", listaJogadores[pos].nome, listaJogadores[pos].apelido);
    printf("O que voce gostaria de mudar?\n");
    printf("  1. O nome\n");
    printf("  2. O apelido\n");
    printf("  0. Nada, voltar\n");
    opcao = lerInteiro("Escolha uma opcao: ");

    switch (opcao) {
        case 1:
            printf("Digite o novo nome: ");
            scanf(" %49[^\n]", listaJogadores[pos].nome);
            tirarPontoVirgula(listaJogadores[pos].nome);
            salvarJogadoresCSV();
            printf("\nTudo certo! O nome agora e: %s\n", listaJogadores[pos].nome);
            break;
        case 2:
            printf("Digite o novo apelido: ");
            scanf(" %29[^\n]", novoApelido);
            tirarPontoVirgula(novoApelido);
            maiusculas(novoApelido);

            // Verifica se o apelido ja pertence a outro jogador
            for (i = 0; i < totalJogadores; i++) {
                if (i != pos && textosIguais(listaJogadores[i].apelido, novoApelido) == 1) {
                    printf("\nEita! Esse apelido ja e de outro jogador, entao mantive o atual. Tente outro, ok?\n");
                    return;
                }
            }
            copiarTexto(listaJogadores[pos].apelido, novoApelido);
            salvarJogadoresCSV();
            printf("\nTudo certo! O apelido agora e: %s\n", listaJogadores[pos].apelido);
            break;
        case 0:
            printf("Sem problemas, nada foi alterado.\n");
            break;
        default:
            printf("Essa opcao nao existe. Escolha 1, 2 ou 0, por favor.\n");
    }
}

// ----------------------------------------------------------------------------
// 4. REMOVER / DESATIVAR JOGADOR
// ----------------------------------------------------------------------------
void removerJogador() {
    int id, pos;

    printf("\n=== Desativar ou reativar um jogador ===\n");

    if (totalJogadores == 0) {
        printf("Ainda nao ha ninguem cadastrado por aqui.\n");
        return;
    }

    id = lerInteiro("Qual e o ID do jogador? ");
    pos = indiceJogadorPorId(id);

    if (pos == -1) {
        printf("Hmm, nao encontrei nenhum jogador com o ID %d. Confira o numero e tente novamente.\n", id);
        return;
    }

    printf("\nJogador: %s (%s) | Situacao atual: ",
           listaJogadores[pos].nome, listaJogadores[pos].apelido);
    if (listaJogadores[pos].ativo == 1) {
        printf("Ativo\n");
    } else {
        printf("Inativo\n");
    }

    if (listaJogadores[pos].ativo == 0) {
        if (confirmar("Esse jogador esta inativo. Quer reativar? (s/n): ")) {
            listaJogadores[pos].ativo = 1;
            salvarJogadoresCSV();
            printf("\nBoas-vindas de volta! O jogador foi reativado.\n");
        } else {
            printf("Tudo bem, nada foi alterado.\n");
        }
    } else {
        if (confirmar("Tem certeza que quer desativar este jogador? (s/n): ")) {
            listaJogadores[pos].ativo = 0;
            salvarJogadoresCSV();
            printf("\nPronto, o jogador foi desativado. Voce pode reativa-lo quando quiser.\n");
        } else {
            printf("Tudo bem, nada foi alterado.\n");
        }
    }
}

// ----------------------------------------------------------------------------
// 5. BUSCAR JOGADOR POR NOME / APELIDO (busca textual com strstr)
// ----------------------------------------------------------------------------
void buscarJogadorPorNome() {
    char termo[50], alvoNome[50], alvoApelido[30];
    int i, achou = 0;

    printf("\n=== Buscar jogador ===\n");

    if (totalJogadores == 0) {
        printf("Ainda nao ha ninguem cadastrado para buscar.\n");
        return;
    }

    printf("Digite um pedacinho do nome ou do apelido: ");
    scanf(" %49[^\n]", termo);
    maiusculas(termo);

    for (i = 0; i < totalJogadores; i++) {
        // Copia e passa para maiusculas para a busca ignorar maiusculas/minusculas
        copiarTexto(alvoNome, listaJogadores[i].nome);
        copiarTexto(alvoApelido, listaJogadores[i].apelido);
        maiusculas(alvoNome);
        maiusculas(alvoApelido);

        if (strstr(alvoNome, termo) != NULL || strstr(alvoApelido, termo) != NULL) {
            if (achou == 0) {
                printf("\n%-4s | %-25s | %-15s | %-7s | %-8s\n",
                       "ID", "NOME", "APELIDO", "FICHAS", "STATUS");
                printf("-----+---------------------------+-----------------+---------+---------\n");
            }
            printf("%-4d | %-25.25s | %-15.15s | %-7d | ",
                   listaJogadores[i].idJogador,
                   listaJogadores[i].nome,
                   listaJogadores[i].apelido,
                   listaJogadores[i].fichasDisponiveis);
            if (listaJogadores[i].ativo == 1) {
                printf("Ativo\n");
            } else {
                printf("Inativo\n");
            }
            achou++;
        }
    }

    if (achou == 0) {
        printf("\nNao achei ninguem com esse termo. Que tal tentar outro pedacinho do nome?\n");
    } else if (achou == 1) {
        printf("\nEncontrei 1 jogador.\n");
    } else {
        printf("\nEncontrei %d jogadores.\n", achou);
    }
}

#endif