#ifndef HIGHSCORES_H
#define HIGHSCORES_H

// ============================================================================
// HIGH SCORES E RELATORIOS ANALITICOS
// ============================================================================

// ----------------------------------------------------------------------------
// PERSISTENCIA: grava (ou regrava) todo o vetor de pontuacoes em highscores.csv
// ----------------------------------------------------------------------------
void salvarHighScoresCSV() {
    FILE *arq;
    int i;

    arq = fopen("highscores.csv", "w"); // modo "w" zera o arquivo e regrava o vetor atualizado
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar highscores.csv! Feche o arquivo se ele estiver aberto no Excel/Bloco de Notas.\n");
        return;
    }

    for (i = 0; i < totalHighScores; i++) {
        fprintf(arq, "%d;%d;%d;%d;%s\n",
                rankingHighScores[i].idRegistro,
                rankingHighScores[i].idJogador,
                rankingHighScores[i].idMaquina,
                rankingHighScores[i].pontuacaoMaxima,
                rankingHighScores[i].dataRegistro);
    }

    fclose(arq);
}

// ----------------------------------------------------------------------------
// PERSISTENCIA: carrega o highscores.csv para o vetor ao abrir o programa
// ----------------------------------------------------------------------------
void carregarHighScoresCSV() {
    FILE *arq;
    char linha[300];
    struct HighScore h;

    arq = fopen("highscores.csv", "r");
    if (arq == NULL) {
        return; // se o arquivo ainda nao existe, comeca com 0 registros
    }

    totalHighScores = 0;
    while (totalHighScores < 50 && lerLinhaCSV(arq, linha, 300)) {
        // %14[^;] para a data nao pegar colunas extras que o Excel possa ter criado
        if (sscanf(linha, "%d;%d;%d;%d;%14[^;]",
                   &h.idRegistro, &h.idJogador, &h.idMaquina,
                   &h.pontuacaoMaxima, h.dataRegistro) == 5) {
            rankingHighScores[totalHighScores] = h;
            totalHighScores++;
        }
    }

    fclose(arq);
}

// Gera o proximo ID de registro de forma direta
int proximoIdRegistro() {
    int i, maior = 0;
    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idRegistro > maior) {
            maior = rankingHighScores[i].idRegistro;
        }
    }
    return maior + 1;
}

// ----------------------------------------------------------------------------
// 1. REGISTRAR NOVA PONTUACAO
// ----------------------------------------------------------------------------
void registrarPontuacao() {
    struct HighScore novo;
    int idJogador, idMaquina, posJ, posM, pontos, i, recordeAnterior = 0, custo;

    printf("\n>> REGISTRAR NOVA PONTUACAO <<\n");

    if (totalHighScores >= 50) {
        printf("[ERRO] Limite de 50 registros atingido!\n");
        return;
    }
    if (totalJogadores == 0 || totalMaquinas == 0) {
        printf("[ERRO] E preciso ter ao menos 1 jogador e 1 maquina cadastrados.\n");
        return;
    }

    idJogador = lerInteiro("ID do jogador: ");
    posJ = indiceJogadorPorId(idJogador);
    if (posJ == -1 || listaJogadores[posJ].ativo == 0) {
        printf("[ERRO] Jogador nao encontrado ou inativo.\n");
        return;
    }

    idMaquina = lerInteiro("ID da maquina: ");
    posM = indiceMaquinaPorId(idMaquina);
    if (posM == -1 || catalogoMaquinas[posM].emOperacao == 0) {
        printf("[ERRO] Maquina nao encontrada ou em manutencao.\n");
        return;
    }

    // Custo da partida em fichas (arredonda para cima) x saldo do jogador
    custo = (int) catalogoMaquinas[posM].precoFicha;
    if (catalogoMaquinas[posM].precoFicha > custo) {
        custo++;
    }
    if (listaJogadores[posJ].fichasDisponiveis < custo) {
        printf("[ERRO] Saldo insuficiente: a partida custa %d ficha(s) e o jogador tem %d.\n",
               custo, listaJogadores[posJ].fichasDisponiveis);
        return;
    }

    pontos = lerInteiro("Pontuacao alcancada: ");
    if (pontos <= 0) {
        printf("[ERRO] A pontuacao deve ser maior que zero.\n");
        return;
    }

    // Verifica recorde anterior na mesma maquina
    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idMaquina == idMaquina &&
            rankingHighScores[i].pontuacaoMaxima > recordeAnterior) {
            recordeAnterior = rankingHighScores[i].pontuacaoMaxima;
        }
    }

    novo.idRegistro = proximoIdRegistro();
    novo.idJogador = idJogador;
    novo.idMaquina = idMaquina;
    novo.pontuacaoMaxima = pontos;
    lerData(novo.dataRegistro);

    rankingHighScores[totalHighScores] = novo;
    totalHighScores++;

    // Debita as fichas da partida do saldo do jogador
    listaJogadores[posJ].fichasDisponiveis -= custo;

    // Grava a pontuacao e o novo saldo do jogador nos arquivos .csv
    salvarHighScoresCSV();
    salvarJogadoresCSV();

    printf("\n[OK] %s fez %d pontos em %s!\n",
           listaJogadores[posJ].apelido, pontos, catalogoMaquinas[posM].nomeJogo);
    printf("Partida custou %d ficha(s). Saldo restante: %d ficha(s).\n",
           custo, listaJogadores[posJ].fichasDisponiveis);

    if (pontos > recordeAnterior) {
        printf("*** NOVO RECORDE DA MAQUINA! ***\n");
    }
}

// ----------------------------------------------------------------------------
// 2. EXIBIR TABELA GERAL DE HIGH SCORES (HALL DA FAMA)
// ----------------------------------------------------------------------------
void exibirHighScores() {
    struct HighScore temp[50];
    struct HighScore t;
    int i, j;

    printf("\n>> HALL DA FAMA - HIGH SCORES <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    // Cria copia simples para ordenar direto na exibicao
    for (i = 0; i < totalHighScores; i++) {
        temp[i] = rankingHighScores[i];
    }

    // Ordenacao Bubble Sort decrescente por pontuacao
    for (i = 0; i < totalHighScores - 1; i++) {
        for (j = 0; j < totalHighScores - 1 - i; j++) {
            if (temp[j].pontuacaoMaxima < temp[j + 1].pontuacaoMaxima) {
                t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    printf("%-4s | %-15s | %-25s | %-10s | %-10s\n",
           "POS", "JOGADOR", "JOGO", "PONTOS", "DATA");
    printf("-----+-----------------+---------------------------+------------+-----------\n");

    for (i = 0; i < totalHighScores; i++) {
        printf("%-4d | ", i + 1);
        mostrarApelido(temp[i].idJogador);
        printf(" | ");
        mostrarNomeMaquina(temp[i].idMaquina);
        printf(" | %-10d | %-10s\n", temp[i].pontuacaoMaxima, temp[i].dataRegistro);
    }
}

// ----------------------------------------------------------------------------
// 3. [RELATORIO] CAMPEAO GERAL DE PONTUACAO
// ----------------------------------------------------------------------------
void campeaoGeral() {
    int i, maior;

    printf("\n>> RELATORIO: CAMPEAO GERAL <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    maior = rankingHighScores[0].pontuacaoMaxima;
    for (i = 1; i < totalHighScores; i++) {
        if (rankingHighScores[i].pontuacaoMaxima > maior) {
            maior = rankingHighScores[i].pontuacaoMaxima;
        }
    }

    // Mostra todos que atingiram a maior pontuacao (cobre empates)
    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].pontuacaoMaxima == maior) {
            printf("RECORDISTA: ");
            mostrarApelido(rankingHighScores[i].idJogador);
            printf(" com %d pontos em ", maior);
            mostrarNomeMaquina(rankingHighScores[i].idMaquina);
            printf(" (%s)\n", rankingHighScores[i].dataRegistro);
        }
    }
}

// ----------------------------------------------------------------------------
// 4. [RELATORIO] JOGO MAIS JOGADO / POPULAR
// ----------------------------------------------------------------------------
void jogoMaisJogado() {
    int contagem[50];
    int i, pos, maior = 0;

    printf("\n>> RELATORIO: JOGO MAIS POPULAR <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    for (i = 0; i < 50; i++) {
        contagem[i] = 0;
    }

    for (i = 0; i < totalHighScores; i++) {
        pos = indiceMaquinaPorId(rankingHighScores[i].idMaquina);
        if (pos != -1) {
            contagem[pos]++;
        }
    }

    printf("%-25s | %-10s\n", "JOGO", "PARTIDAS");
    printf("--------------------------+-----------\n");
    for (i = 0; i < totalMaquinas; i++) {
        printf("%-25.25s | %-10d\n", catalogoMaquinas[i].nomeJogo, contagem[i]);
        if (contagem[i] > maior) {
            maior = contagem[i];
        }
    }

    printf("\n");
    for (i = 0; i < totalMaquinas; i++) {
        if (contagem[i] == maior && maior > 0) {
            printf("MAIS POPULAR: %s (%d partida(s))\n",
                   catalogoMaquinas[i].nomeJogo, maior);
        }
    }
}

// ----------------------------------------------------------------------------
// 5. [RELATORIO] HISTORICO POR JOGADOR
// ----------------------------------------------------------------------------
void historicoPorJogador() {
    int id, pos, i, qtd = 0, melhor = 0;

    printf("\n>> RELATORIO: HISTORICO DO JOGADOR <<\n");

    if (totalJogadores == 0) {
        printf("Nenhum jogador cadastrado ainda.\n");
        return;
    }

    id = lerInteiro("Digite o ID do jogador: ");
    pos = indiceJogadorPorId(id);
    if (pos == -1) {
        printf("[ERRO] Jogador nao encontrado.\n");
        return;
    }

    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idJogador == id &&
            rankingHighScores[i].pontuacaoMaxima > melhor) {
            melhor = rankingHighScores[i].pontuacaoMaxima;
        }
    }

    printf("Jogador: %s (%s)\n\n", listaJogadores[pos].nome, listaJogadores[pos].apelido);
    printf("%-25s | %-10s | %-10s\n", "JOGO", "PONTOS", "DATA");
    printf("--------------------------+------------+-----------\n");

    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idJogador == id) {
            mostrarNomeMaquina(rankingHighScores[i].idMaquina);
            printf(" | %-10d | %-10s", rankingHighScores[i].pontuacaoMaxima,
                   rankingHighScores[i].dataRegistro);
            if (rankingHighScores[i].pontuacaoMaxima == melhor) {
                printf("  <- MELHOR");
            }
            printf("\n");
            qtd++;
        }
    }

    if (qtd == 0) {
        printf("Esse jogador ainda nao tem pontuacoes registradas.\n");
    } else {
        printf("\n%d registro(s) | Melhor pontuacao: %d\n", qtd, melhor);
    }
}

// ----------------------------------------------------------------------------
// 6. BUSCAR PONTUACOES POR JOGADOR / JOGO (busca textual com strstr)
// ----------------------------------------------------------------------------
void buscarPontuacao() {
    char termo[40], alvoApelido[30], alvoJogo[40];
    int i, posJ, posM, achou = 0;

    printf("\n>> BUSCAR PONTUACOES <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    printf("Digite parte do apelido do jogador ou do nome do jogo: ");
    scanf(" %39[^\n]", termo);
    maiusculas(termo);

    for (i = 0; i < totalHighScores; i++) {
        posJ = indiceJogadorPorId(rankingHighScores[i].idJogador);
        posM = indiceMaquinaPorId(rankingHighScores[i].idMaquina);
        if (posJ == -1 || posM == -1) {
            continue;
        }

        // Copia e passa para maiusculas para a busca ignorar maiusculas/minusculas
        copiarTexto(alvoApelido, listaJogadores[posJ].apelido);
        copiarTexto(alvoJogo, catalogoMaquinas[posM].nomeJogo);
        maiusculas(alvoApelido);
        maiusculas(alvoJogo);

        if (strstr(alvoApelido, termo) != NULL || strstr(alvoJogo, termo) != NULL) {
            if (achou == 0) {
                printf("\n%-4s | %-15s | %-25s | %-10s | %-10s\n",
                       "REG.", "JOGADOR", "JOGO", "PONTOS", "DATA");
                printf("-----+-----------------+---------------------------+------------+-----------\n");
            }
            printf("%-4d | ", rankingHighScores[i].idRegistro);
            mostrarApelido(rankingHighScores[i].idJogador);
            printf(" | ");
            mostrarNomeMaquina(rankingHighScores[i].idMaquina);
            printf(" | %-10d | %-10s\n",
                   rankingHighScores[i].pontuacaoMaxima,
                   rankingHighScores[i].dataRegistro);
            achou++;
        }
    }

    if (achou == 0) {
        printf("Nenhuma pontuacao encontrada para esse termo.\n");
    } else {
        printf("\n%d registro(s) encontrado(s).\n", achou);
    }
}

// ----------------------------------------------------------------------------
// 7. EDITAR PONTUACAO REGISTRADA (pontos e data)
// ----------------------------------------------------------------------------
void editarPontuacao() {
    int id, pos, i, pontos;

    printf("\n>> EDITAR PONTUACAO <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    id = lerInteiro("Digite o ID do registro (coluna REG.): ");

    pos = -1;
    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idRegistro == id) {
            pos = i;
        }
    }

    if (pos == -1) {
        printf("[ERRO] Registro com ID %d nao encontrado.\n", id);
        return;
    }

    printf("Registro #%d | ", rankingHighScores[pos].idRegistro);
    mostrarApelido(rankingHighScores[pos].idJogador);
    printf(" | ");
    mostrarNomeMaquina(rankingHighScores[pos].idMaquina);
    printf(" | %d pontos | %s\n", rankingHighScores[pos].pontuacaoMaxima,
           rankingHighScores[pos].dataRegistro);

    pontos = lerInteiro("Nova pontuacao: ");
    if (pontos <= 0) {
        printf("[ERRO] A pontuacao deve ser maior que zero.\n");
        return;
    }

    rankingHighScores[pos].pontuacaoMaxima = pontos;
    lerData(rankingHighScores[pos].dataRegistro);

    salvarHighScoresCSV();
    printf("[OK] Pontuacao atualizada para %d pontos (%s).\n",
           rankingHighScores[pos].pontuacaoMaxima, rankingHighScores[pos].dataRegistro);
}

// ----------------------------------------------------------------------------
// 8. REMOVER PONTUACAO REGISTRADA
// ----------------------------------------------------------------------------
void removerPontuacao() {
    int id, pos, i;

    printf("\n>> REMOVER PONTUACAO <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    id = lerInteiro("Digite o ID do registro (coluna REG.): ");

    pos = -1;
    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idRegistro == id) {
            pos = i;
        }
    }

    if (pos == -1) {
        printf("[ERRO] Registro com ID %d nao encontrado.\n", id);
        return;
    }

    printf("Registro #%d | ", rankingHighScores[pos].idRegistro);
    mostrarApelido(rankingHighScores[pos].idJogador);
    printf(" | ");
    mostrarNomeMaquina(rankingHighScores[pos].idMaquina);
    printf(" | %d pontos | %s\n", rankingHighScores[pos].pontuacaoMaxima,
           rankingHighScores[pos].dataRegistro);

    if (confirmar("Remover essa pontuacao? (s/n): ")) {
        // Desloca os elementos restantes no vetor
        for (i = pos; i < totalHighScores - 1; i++) {
            rankingHighScores[i] = rankingHighScores[i + 1];
        }
        totalHighScores--;
        salvarHighScoresCSV(); // regrava o CSV sem o registro removido
        printf("[OK] Pontuacao removida com sucesso.\n");
    } else {
        printf("Operacao cancelada.\n");
    }
}

// ----------------------------------------------------------------------------
// 9. [RELATORIO] MATRIZ 2D JOGADOR x MAQUINA (MELHOR PONTUACAO)
// ----------------------------------------------------------------------------
void matrizJogadorMaquina() {
    int matriz[50][50]; // linha = posicao do jogador | coluna = posicao da maquina
    int i, j, k, posJ, posM, recorde, posRecorde;

    printf("\n>> RELATORIO: MATRIZ JOGADOR x MAQUINA (MELHOR PONTUACAO) <<\n");

    if (totalHighScores == 0) {
        printf("Nenhuma pontuacao registrada ainda.\n");
        return;
    }

    // Zera a matriz
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            matriz[i][j] = 0;
        }
    }

    // Preenche cada celula com a melhor pontuacao do jogador naquela maquina
    for (i = 0; i < totalHighScores; i++) {
        posJ = indiceJogadorPorId(rankingHighScores[i].idJogador);
        posM = indiceMaquinaPorId(rankingHighScores[i].idMaquina);
        if (posJ != -1 && posM != -1 &&
            rankingHighScores[i].pontuacaoMaxima > matriz[posJ][posM]) {
            matriz[posJ][posM] = rankingHighScores[i].pontuacaoMaxima;
        }
    }

    // Cabecalho (colunas = IDs das maquinas)
    printf("\n%-3s %-10s ", "ID", "JOGADOR");
    for (j = 0; j < totalMaquinas; j++) {
        printf("M%-5d ", catalogoMaquinas[j].idMaquina);
    }
    printf("\n");
    for (k = 0; k < 14 + 7 * totalMaquinas; k++) {
        printf("-");
    }
    printf("\n");

    // Linhas (um jogador por linha)
    for (i = 0; i < totalJogadores; i++) {
        printf("%-3d %-10.10s ", listaJogadores[i].idJogador, listaJogadores[i].apelido);
        for (j = 0; j < totalMaquinas; j++) {
            if (matriz[i][j] == 0) {
                printf("%-6s ", "-");
            } else {
                printf("%-6d ", matriz[i][j]);
            }
        }
        printf("\n");
    }

    // Ultima linha: recorde de cada maquina (maior valor de cada coluna)
    for (k = 0; k < 14 + 7 * totalMaquinas; k++) {
        printf("-");
    }
    printf("\n%-3s %-10s ", "", "RECORDE");
    for (j = 0; j < totalMaquinas; j++) {
        recorde = 0;
        for (i = 0; i < totalJogadores; i++) {
            if (matriz[i][j] > recorde) {
                recorde = matriz[i][j];
            }
        }
        if (recorde == 0) {
            printf("%-6s ", "-");
        } else {
            printf("%-6d ", recorde);
        }
    }
    printf("\n");

    // Quem detem o recorde de cada maquina
    printf("\nRecordistas por maquina:\n");
    for (j = 0; j < totalMaquinas; j++) {
        recorde = 0;
        posRecorde = -1;
        for (i = 0; i < totalJogadores; i++) {
            if (matriz[i][j] > recorde) {
                recorde = matriz[i][j];
                posRecorde = i;
            }
        }
        printf("M%-3d = %-25.25s ", catalogoMaquinas[j].idMaquina, catalogoMaquinas[j].nomeJogo);
        if (posRecorde == -1) {
            printf("(sem pontuacoes)\n");
        } else {
            printf("%s com %d pontos\n", listaJogadores[posRecorde].apelido, recorde);
        }
    }
}

#endif
