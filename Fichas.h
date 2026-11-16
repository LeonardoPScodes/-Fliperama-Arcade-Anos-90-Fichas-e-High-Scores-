#ifndef FICHAS_H
#define FICHAS_H

// ============================================================================
// CAIXA E VENDA DE FICHAS
// ============================================================================

// ----------------------------------------------------------------------------
// PERSISTENCIA: grava (ou regrava) todo o historico de vendas em fichas.csv
// ----------------------------------------------------------------------------
void salvarFichasCSV() {
    FILE *arq;
    int i;

    arq = fopen("fichas.csv", "w"); // modo "w" zera o arquivo e regrava o vetor atualizado
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar fichas.csv! Feche o arquivo se ele estiver aberto no Excel/Bloco de Notas.\n");
        return;
    }

    for (i = 0; i < totalVendasFichas; i++) {
        fprintf(arq, "%d;%d;%d;%.2f;%s\n",
                historicoFichas[i].idTransacao,
                historicoFichas[i].idJogadorComprador,
                historicoFichas[i].quantidadeFichas,
                historicoFichas[i].valorTotalPago,
                historicoFichas[i].dataTransacao);
    }

    fclose(arq);
}

// ----------------------------------------------------------------------------
// PERSISTENCIA: carrega o fichas.csv para o vetor ao abrir o programa
// ----------------------------------------------------------------------------
void carregarFichasCSV() {
    FILE *arq;
    char linha[300];
    struct VendaFichas v;

    arq = fopen("fichas.csv", "r");
    if (arq == NULL) {
        return; // se o arquivo ainda nao existe, comeca com 0 registros
    }

    totalVendasFichas = 0;
    while (totalVendasFichas < 50 && lerLinhaCSV(arq, linha, 300)) {
        virgulaParaPonto(linha); // aceita valor no formato 40,00 (Excel em portugues)
        if (sscanf(linha, "%d;%d;%d;%f;%14s",
                   &v.idTransacao, &v.idJogadorComprador, &v.quantidadeFichas,
                   &v.valorTotalPago, v.dataTransacao) == 5) {
            historicoFichas[totalVendasFichas] = v;
            totalVendasFichas++;
        }
    }

    fclose(arq);
}

// Gera o proximo ID de transacao de forma direta
int proximoIdTransacao() {
    int i, maior = 0;
    for (i = 0; i < totalVendasFichas; i++) {
        if (historicoFichas[i].idTransacao > maior) {
            maior = historicoFichas[i].idTransacao;
        }
    }
    return maior + 1;
}

// ----------------------------------------------------------------------------
// 1. VENDER FICHAS PARA UM JOGADOR
// ----------------------------------------------------------------------------
void venderFichas() {
    struct VendaFichas nova;
    int idJogador, pos, qtd;
    float valor;

    printf("\n>> VENDA DE FICHAS <<\n");

    if (totalVendasFichas >= 50) {
        printf("[ERRO] Limite de 50 vendas atingido!\n");
        return;
    }
    if (totalJogadores == 0) {
        printf("[ERRO] Nenhum jogador cadastrado.\n");
        return;
    }

    idJogador = lerInteiro("ID do jogador comprador: ");
    pos = indiceJogadorPorId(idJogador);

    if (pos == -1 || listaJogadores[pos].ativo == 0) {
        printf("[ERRO] Jogador nao encontrado ou inativo.\n");
        return;
    }

    printf("Jogador: %s | Saldo atual: %d ficha(s)\n",
           listaJogadores[pos].apelido, listaJogadores[pos].fichasDisponiveis);
    printf("Preco da ficha: R$ %.2f\n", PRECO_FICHA);

    qtd = lerInteiro("Quantidade de fichas: ");
    if (qtd <= 0 || qtd > 1000) {
        printf("[ERRO] Quantidade invalida (1 a 1000).\n");
        return;
    }

    valor = qtd * PRECO_FICHA;
    printf("Total a pagar: R$ %.2f\n", valor);

    if (confirmar("Confirmar venda? (s/n): ") == 0) {
        printf("Venda cancelada.\n");
        return;
    }

    // Registra a nova venda
    nova.idTransacao = proximoIdTransacao();
    nova.idJogadorComprador = idJogador;
    nova.quantidadeFichas = qtd;
    nova.valorTotalPago = valor;
    lerData(nova.dataTransacao);

    historicoFichas[totalVendasFichas] = nova;
    totalVendasFichas++;
    listaJogadores[pos].fichasDisponiveis += qtd;

    // Grava a venda e o novo saldo do jogador nos arquivos .csv
    salvarFichasCSV();
    salvarJogadoresCSV();

    printf("[OK] Venda #%d realizada com sucesso!\n", nova.idTransacao);
}

// ----------------------------------------------------------------------------
// 2. CONSULTAR HISTORICO DE VENDAS
// ----------------------------------------------------------------------------
void consultarHistoricoVendas() {
    int i;

    printf("\n>> HISTORICO DE VENDAS <<\n");

    if (totalVendasFichas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    printf("%-6s | %-15s | %-7s | %-12s | %-10s\n",
           "TRANS.", "JOGADOR", "FICHAS", "VALOR (R$)", "DATA");
    printf("-------+-----------------+---------+--------------+-----------\n");

    for (i = 0; i < totalVendasFichas; i++) {
        printf("%-6d | ", historicoFichas[i].idTransacao);
        mostrarApelido(historicoFichas[i].idJogadorComprador);
        printf(" | %-7d | %-12.2f | %-10s\n",
               historicoFichas[i].quantidadeFichas,
               historicoFichas[i].valorTotalPago,
               historicoFichas[i].dataTransacao);
    }
}

// ----------------------------------------------------------------------------
// 3. ESTORNAR VENDA DE FICHAS
// ----------------------------------------------------------------------------
void estornarVenda() {
    int id, posVenda = -1, posJogador, qtdFichas, i;

    printf("\n>> ESTORNO DE VENDA <<\n");

    if (totalVendasFichas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    id = lerInteiro("Digite o ID da transacao para estornar: ");

    // Busca a venda pelo ID
    for (i = 0; i < totalVendasFichas; i++) {
        if (historicoFichas[i].idTransacao == id) {
            posVenda = i;
        }
    }

    if (posVenda == -1) {
        printf("[ERRO] Transacao nao encontrada.\n");
        return;
    }

    posJogador = indiceJogadorPorId(historicoFichas[posVenda].idJogadorComprador);
    if (posJogador == -1) {
        printf("[ERRO] Jogador associado nao existe mais.\n");
        return;
    }

    qtdFichas = historicoFichas[posVenda].quantidadeFichas;
    if (listaJogadores[posJogador].fichasDisponiveis < qtdFichas) {
        printf("[ERRO] O jogador ja utilizou parte das fichas e nao pode estornar.\n");
        return;
    }

    if (confirmar("Confirmar estorno? (s/n): ") == 0) {
        printf("Estorno cancelado.\n");
        return;
    }

    // Remove as fichas do jogador
    listaJogadores[posJogador].fichasDisponiveis -= qtdFichas;

    // Remove a venda do vetor deslocando os elementos restantes
    for (i = posVenda; i < totalVendasFichas - 1; i++) {
        historicoFichas[i] = historicoFichas[i + 1];
    }
    totalVendasFichas--;

    // Regrava os arquivos .csv atualizados
    salvarFichasCSV();
    salvarJogadoresCSV();

    printf("[OK] Venda estornada com sucesso.\n");
}

// ----------------------------------------------------------------------------
// 4. FATURAMENTO TOTAL DO CAIXA
// ----------------------------------------------------------------------------
void faturamentoTotal() {
    int i, totalFichas = 0;
    float faturamento = 0;

    printf("\n>> RELATORIO: FATURAMENTO <<\n");

    if (totalVendasFichas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    for (i = 0; i < totalVendasFichas; i++) {
        faturamento += historicoFichas[i].valorTotalPago;
        totalFichas += historicoFichas[i].quantidadeFichas;
    }

    printf("Vendas realizadas : %d\n", totalVendasFichas);
    printf("Fichas vendidas   : %d\n", totalFichas);
    printf("Faturamento total : R$ %.2f\n", faturamento);
    printf("Ticket medio      : R$ %.2f\n", faturamento / totalVendasFichas);
}

// ----------------------------------------------------------------------------
// 5. BUSCAR VENDAS POR JOGADOR (busca textual com strstr no apelido)
// ----------------------------------------------------------------------------
void buscarVendasPorJogador() {
    char termo[30], alvo[30];
    int i, pos, achou = 0;

    printf("\n>> BUSCAR VENDAS POR JOGADOR <<\n");

    if (totalVendasFichas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    printf("Digite parte do apelido do jogador: ");
    scanf(" %29[^\n]", termo);
    maiusculas(termo);

    for (i = 0; i < totalVendasFichas; i++) {
        pos = indiceJogadorPorId(historicoFichas[i].idJogadorComprador);
        if (pos == -1) {
            continue;
        }

        // Copia e passa para maiusculas para a busca ignorar maiusculas/minusculas
        copiarTexto(alvo, listaJogadores[pos].apelido);
        maiusculas(alvo);

        if (strstr(alvo, termo) != NULL) {
            if (achou == 0) {
                printf("\n%-6s | %-15s | %-7s | %-12s | %-10s\n",
                       "TRANS.", "JOGADOR", "FICHAS", "VALOR (R$)", "DATA");
                printf("-------+-----------------+---------+--------------+-----------\n");
            }
            printf("%-6d | ", historicoFichas[i].idTransacao);
            mostrarApelido(historicoFichas[i].idJogadorComprador);
            printf(" | %-7d | %-12.2f | %-10s\n",
                   historicoFichas[i].quantidadeFichas,
                   historicoFichas[i].valorTotalPago,
                   historicoFichas[i].dataTransacao);
            achou++;
        }
    }

    if (achou == 0) {
        printf("Nenhuma venda encontrada para esse termo.\n");
    } else {
        printf("\n%d venda(s) encontrada(s).\n", achou);
    }
}

// ----------------------------------------------------------------------------
// 6. EDITAR VENDA DE FICHAS (quantidade ou data)
// ----------------------------------------------------------------------------
void editarVenda() {
    int id, posVenda = -1, posJogador, opcao, novaQtd, diferenca, i;

    printf("\n>> EDITAR VENDA DE FICHAS <<\n");

    if (totalVendasFichas == 0) {
        printf("Nenhuma venda registrada.\n");
        return;
    }

    id = lerInteiro("Digite o ID da transacao para editar: ");

    // Busca a venda pelo ID
    for (i = 0; i < totalVendasFichas; i++) {
        if (historicoFichas[i].idTransacao == id) {
            posVenda = i;
        }
    }

    if (posVenda == -1) {
        printf("[ERRO] Transacao nao encontrada.\n");
        return;
    }

    posJogador = indiceJogadorPorId(historicoFichas[posVenda].idJogadorComprador);
    if (posJogador == -1) {
        printf("[ERRO] Jogador associado nao existe mais.\n");
        return;
    }

    printf("Venda #%d | Jogador: %s | %d ficha(s) | R$ %.2f | %s\n",
           historicoFichas[posVenda].idTransacao,
           listaJogadores[posJogador].apelido,
           historicoFichas[posVenda].quantidadeFichas,
           historicoFichas[posVenda].valorTotalPago,
           historicoFichas[posVenda].dataTransacao);
    printf("1. Alterar quantidade de fichas\n");
    printf("2. Alterar data\n");
    printf("0. Cancelar\n");
    opcao = lerInteiro("Escolha: ");

    switch (opcao) {
        case 1:
            novaQtd = lerInteiro("Nova quantidade de fichas: ");
            if (novaQtd <= 0 || novaQtd > 1000) {
                printf("[ERRO] Quantidade invalida (1 a 1000).\n");
                return;
            }

            // Ajusta o saldo do jogador pela diferenca entre a quantidade nova e a antiga
            diferenca = novaQtd - historicoFichas[posVenda].quantidadeFichas;
            if (listaJogadores[posJogador].fichasDisponiveis + diferenca < 0) {
                printf("[ERRO] O jogador ja utilizou fichas demais para reduzir essa venda.\n");
                return;
            }

            listaJogadores[posJogador].fichasDisponiveis += diferenca;
            historicoFichas[posVenda].quantidadeFichas = novaQtd;
            historicoFichas[posVenda].valorTotalPago = novaQtd * PRECO_FICHA;

            salvarFichasCSV();
            salvarJogadoresCSV();
            printf("[OK] Venda atualizada: %d ficha(s) | R$ %.2f | Novo saldo do jogador: %d\n",
                   novaQtd, historicoFichas[posVenda].valorTotalPago,
                   listaJogadores[posJogador].fichasDisponiveis);
            break;
        case 2:
            lerData(historicoFichas[posVenda].dataTransacao);
            salvarFichasCSV();
            printf("[OK] Data atualizada: %s\n", historicoFichas[posVenda].dataTransacao);
            break;
        case 0:
            printf("Operacao cancelada.\n");
            break;
        default:
            printf("[ERRO] Opcao invalida!\n");
    }
}

#endif
