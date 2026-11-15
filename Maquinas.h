#ifndef MAQUINAS_H
#define MAQUINAS_H

void salvarMaquinasCSV() {
    FILE *arq;
    int i;

    arq = fopen("maquinas.csv", "w");
    if (arq == NULL) {
        printf("[ERRO] Nao foi possivel gravar maquinas.csv! Feche o arquivo se ele estiver aberto no Excel/Bloco de Notas.\n");
        return;
    }

    for (i = 0; i < totalMaquinas; i++) {
        fprintf(arq, "%d;%s;%s;%.2f;%d\n",
                catalogoMaquinas[i].idMaquina,
                catalogoMaquinas[i].nomeJogo,
                catalogoMaquinas[i].genero,
                catalogoMaquinas[i].precoFicha,
                catalogoMaquinas[i].emOperacao);
    }

    fclose(arq);
}

void carregarMaquinasCSV() {
    FILE *arq;
    char linha[300];
    struct MaquinaArcade m;

    arq = fopen("maquinas.csv", "r");
    if (arq == NULL) {
        return;
    }

    totalMaquinas = 0;
    while (totalMaquinas < 50 && lerLinhaCSV(arq, linha, 300)) {
        virgulaParaPonto(linha); 
        if (sscanf(linha, "%d;%39[^;];%29[^;];%f;%d",
                   &m.idMaquina, m.nomeJogo, m.genero,
                   &m.precoFicha, &m.emOperacao) == 5) {
            catalogoMaquinas[totalMaquinas] = m;
            totalMaquinas++;
        }
    }

    fclose(arq);
}

int proximoIdMaquina() {
    int i, maior = 0;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].idMaquina > maior) {
            maior = catalogoMaquinas[i].idMaquina;
        }
    }
    return maior + 1;
}

int pontuacoesDaMaquina(int idMaquina) {
    int i, total = 0;
    for (i = 0; i < totalHighScores; i++) {
        if (rankingHighScores[i].idMaquina == idMaquina) {
            total++;
        }
    }
    return total;
}

void cadastrarMaquina() {
    struct MaquinaArcade nova;
    int i;

    printf("\n>> CADASTRO DE NOVA MAQUINA <<\n");

    if (totalMaquinas >= 50) {
        printf("[ERRO] Limite de 50 maquinas atingido!\n");
        return;
    }

    nova.idMaquina = proximoIdMaquina();

    printf("Nome do jogo (ex: Street Fighter II): ");
    scanf(" %39[^\n]", nova.nomeJogo);
    tirarPontoVirgula(nova.nomeJogo);

    // Verifica se ja existe maquina com o mesmo nome
    for (i = 0; i < totalMaquinas; i++) {
        if (textosIguais(catalogoMaquinas[i].nomeJogo, nova.nomeJogo) == 1) {
            printf("[ERRO] Ja existe uma maquina cadastrada com esse nome.\n");
            return;
        }
    }

    printf("Genero (ex: Luta, Tiro): ");
    scanf(" %29[^\n]", nova.genero);
    tirarPontoVirgula(nova.genero);

    nova.precoFicha = lerDecimal("Preco por partida (em fichas): ");
    if (nova.precoFicha <= 0) {
        printf("[ERRO] O preco deve ser maior que zero.\n");
        return;
    }

    nova.emOperacao = 1;
    catalogoMaquinas[totalMaquinas] = nova;
    totalMaquinas++;
    salvarMaquinasCSV();

    printf("\n[OK] Maquina cadastrada! ID: %d | %s (%s)\n",
           nova.idMaquina, nova.nomeJogo, nova.genero);
    printf("[OK] Dados salvos em 'maquinas.csv'.\n");
}

// ----------------------------------------------------------------------------
// 2. LISTAR GABINETES
// ----------------------------------------------------------------------------
void listarMaquinas() {
    int i, operando = 0;

    printf("\n>> CATALOGO DE MAQUINAS <<\n");

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada ainda.\n");
        return;
    }

    printf("%-4s | %-25s | %-15s | %-14s | %-12s\n",
           "ID", "JOGO", "GENERO", "FICHAS/PARTIDA", "STATUS");
    printf("-----+---------------------------+-----------------+----------------+-------------\n");

    for (i = 0; i < totalMaquinas; i++) {
        printf("%-4d | %-25.25s | %-15.15s | %-14.2f | ",
               catalogoMaquinas[i].idMaquina,
               catalogoMaquinas[i].nomeJogo,
               catalogoMaquinas[i].genero,
               catalogoMaquinas[i].precoFicha);
        if (catalogoMaquinas[i].emOperacao == 1) {
            printf("Operando\n");
            operando++;
        } else {
            printf("Manutencao\n");
        }
    }

    printf("\nTotal: %d maquina(s) | %d operando | %d em manutencao\n",
           totalMaquinas, operando, totalMaquinas - operando);
}

// ----------------------------------------------------------------------------
// 3. ALTERAR STATUS (OPERANDO <-> MANUTENCAO)
// ----------------------------------------------------------------------------
void alterarStatusMaquina() {
    int id, pos, resposta;

    printf("\n>> ALTERAR STATUS DA MAQUINA <<\n");

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada ainda.\n");
        return;
    }

    id = lerInteiro("Digite o ID da maquina: ");
    pos = indiceMaquinaPorId(id);

    if (pos == -1) {
        printf("[ERRO] Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    if (catalogoMaquinas[pos].emOperacao == 1) {
        printf("Maquina: %s | Status atual: Operando\n", catalogoMaquinas[pos].nomeJogo);
        resposta = confirmar("Mudar para MANUTENCAO? (s/n): ");
    } else {
        printf("Maquina: %s | Status atual: Em manutencao\n", catalogoMaquinas[pos].nomeJogo);
        resposta = confirmar("Mudar para OPERANDO? (s/n): ");
    }

    if (resposta == 1) {
        if (catalogoMaquinas[pos].emOperacao == 1) {
            catalogoMaquinas[pos].emOperacao = 0;
        } else {
            catalogoMaquinas[pos].emOperacao = 1;
        }
        salvarMaquinasCSV();
        printf("[OK] Status alterado com sucesso.\n");
    } else {
        printf("Operacao cancelada.\n");
    }
}

// ----------------------------------------------------------------------------
// 4. REMOVER MAQUINA DEFINITIVAMENTE
// ----------------------------------------------------------------------------
void removerMaquina() {
    int id, pos, i, qtdScores;

    printf("\n>> REMOVER MAQUINA <<\n");

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada ainda.\n");
        return;
    }

    id = lerInteiro("Digite o ID da maquina: ");
    pos = indiceMaquinaPorId(id);

    if (pos == -1) {
        printf("[ERRO] Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    qtdScores = pontuacoesDaMaquina(id);
    if (qtdScores > 0) {
        printf("[ERRO] A maquina tem %d pontuacao(oes) registrada(s) e nao pode ser removida.\n", qtdScores);
        printf("Dica: Use a opcao de alterar status para coloca-la em manutencao.\n");
        return;
    }

    printf("Maquina: %s (%s)\n", catalogoMaquinas[pos].nomeJogo, catalogoMaquinas[pos].genero);

    if (confirmar("Remover DEFINITIVAMENTE? (s/n): ")) {
        // Desloca os elementos restantes no vetor
        for (i = pos; i < totalMaquinas - 1; i++) {
            catalogoMaquinas[i] = catalogoMaquinas[i + 1];
        }
        totalMaquinas--;
        salvarMaquinasCSV(); // regrava o CSV sem a maquina removida
        printf("[OK] Maquina removida com sucesso.\n");
    } else {
        printf("Operacao cancelada.\n");
    }
}

// ----------------------------------------------------------------------------
// 5. EDITAR DADOS DA MAQUINA (nome, genero ou preco por partida)
// ----------------------------------------------------------------------------
void editarMaquina() {
    int id, pos, opcao, i;
    char novoTexto[40];
    float novoPreco;

    printf("\n>> EDITAR DADOS DA MAQUINA <<\n");

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada ainda.\n");
        return;
    }

    id = lerInteiro("Digite o ID da maquina: ");
    pos = indiceMaquinaPorId(id);

    if (pos == -1) {
        printf("[ERRO] Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    printf("Maquina atual: %s (%s) | %.2f ficha(s) por partida\n",
           catalogoMaquinas[pos].nomeJogo, catalogoMaquinas[pos].genero,
           catalogoMaquinas[pos].precoFicha);
    printf("1. Alterar nome do jogo\n");
    printf("2. Alterar genero\n");
    printf("3. Alterar preco por partida\n");
    printf("0. Cancelar\n");
    opcao = lerInteiro("Escolha: ");

    switch (opcao) {
        case 1:
            printf("Novo nome do jogo: ");
            scanf(" %39[^\n]", novoTexto);
            tirarPontoVirgula(novoTexto);

            // Verifica se o nome ja pertence a outra maquina
            for (i = 0; i < totalMaquinas; i++) {
                if (i != pos && textosIguais(catalogoMaquinas[i].nomeJogo, novoTexto) == 1) {
                    printf("[ERRO] Ja existe outra maquina com esse nome. Nome nao alterado.\n");
                    return;
                }
            }
            copiarTexto(catalogoMaquinas[pos].nomeJogo, novoTexto);
            salvarMaquinasCSV();
            printf("[OK] Nome atualizado: %s\n", catalogoMaquinas[pos].nomeJogo);
            break;
        case 2:
            printf("Novo genero: ");
            scanf(" %29[^\n]", novoTexto);
            tirarPontoVirgula(novoTexto);
            copiarTexto(catalogoMaquinas[pos].genero, novoTexto);
            salvarMaquinasCSV();
            printf("[OK] Genero atualizado: %s\n", catalogoMaquinas[pos].genero);
            break;
        case 3:
            novoPreco = lerDecimal("Novo preco por partida (em fichas): ");
            if (novoPreco <= 0) {
                printf("[ERRO] O preco deve ser maior que zero.\n");
                return;
            }
            catalogoMaquinas[pos].precoFicha = novoPreco;
            salvarMaquinasCSV();
            printf("[OK] Preco atualizado: %.2f ficha(s) por partida\n", catalogoMaquinas[pos].precoFicha);
            break;
        case 0:
            printf("Operacao cancelada.\n");
            break;
        default:
            printf("[ERRO] Opcao invalida!\n");
    }
}

// ----------------------------------------------------------------------------
// 6. BUSCAR MAQUINA POR NOME / GENERO (busca textual com strstr)
// ----------------------------------------------------------------------------
void buscarMaquinaPorNome() {
    char termo[40], alvoNome[40], alvoGenero[30];
    int i, achou = 0;

    printf("\n>> BUSCAR MAQUINA <<\n");

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada ainda.\n");
        return;
    }

    printf("Digite parte do nome do jogo ou do genero: ");
    scanf(" %39[^\n]", termo);
    maiusculas(termo);

    for (i = 0; i < totalMaquinas; i++) {
        // Copia e passa para maiusculas para a busca ignorar maiusculas/minusculas
        copiarTexto(alvoNome, catalogoMaquinas[i].nomeJogo);
        copiarTexto(alvoGenero, catalogoMaquinas[i].genero);
        maiusculas(alvoNome);
        maiusculas(alvoGenero);

        if (strstr(alvoNome, termo) != NULL || strstr(alvoGenero, termo) != NULL) {
            if (achou == 0) {
                printf("\n%-4s | %-25s | %-15s | %-14s | %-12s\n",
                       "ID", "JOGO", "GENERO", "FICHAS/PARTIDA", "STATUS");
                printf("-----+---------------------------+-----------------+----------------+-------------\n");
            }
            printf("%-4d | %-25.25s | %-15.15s | %-14.2f | ",
                   catalogoMaquinas[i].idMaquina,
                   catalogoMaquinas[i].nomeJogo,
                   catalogoMaquinas[i].genero,
                   catalogoMaquinas[i].precoFicha);
            if (catalogoMaquinas[i].emOperacao == 1) {
                printf("Operando\n");
            } else {
                printf("Manutencao\n");
            }
            achou++;
        }
    }

    if (achou == 0) {
        printf("Nenhuma maquina encontrada com esse termo.\n");
    } else {
        printf("\n%d maquina(s) encontrada(s).\n", achou);
    }
}

#endif
