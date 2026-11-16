#ifndef UTILS_H
#define UTILS_H

// ============================================================================
// UTILS.H - Funcoes auxiliares
// ============================================================================

#include <stdio.h>
#include <stdlib.h>   // system()
#include <string.h>   // strstr()

float PRECO_FICHA = 2.00; // Valor de cada ficha em reais (R$)

// Le um inteiro (repete ate o usuario digitar um numero)
int lerInteiro(char mensagem[]) {
    int valor, r;
    printf("%s", mensagem);
    r = scanf("%d", &valor);
    while (r != 1) {
        if (r == -1) {
            return 0; // fim da entrada de dados
        }
        scanf("%*s"); // descarta o que foi digitado errado
        printf("[ERRO] Digite um numero valido: ");
        r = scanf("%d", &valor);
    }
    return valor;
}

// Le um numero decimal (repete ate o usuario digitar um numero)
float lerDecimal(char mensagem[]) {
    float valor;
    int r;
    printf("%s", mensagem);
    r = scanf("%f", &valor);
    while (r != 1) {
        if (r == -1) {
            return 0;
        }
        scanf("%*s");
        printf("[ERRO] Digite um valor valido (ex: 2.50): ");
        r = scanf("%f", &valor);
    }
    return valor;
}

// Le a data digitada (dd/mm/aaaa)
void lerData(char destino[]) {
    printf("Data (dd/mm/aaaa): ");
    scanf(" %14s", destino);
}

// Pergunta (s/n). Retorna 1 se a resposta for 's' ou 'S'
int confirmar(char mensagem[]) {
    char resp;
    printf("%s", mensagem);
    scanf(" %c", &resp);
    if (resp == 's' || resp == 'S') {
        return 1;
    } else {
        return 0;
    }
}

// Devolve a letra em minuscula
char minuscula(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    } else {
        return c;
    }
}

// Passa o texto inteiro para MAIUSCULAS
void maiusculas(char texto[]) {
    int i = 0;
    while (texto[i] != '\0') {
        if (texto[i] >= 'a' && texto[i] <= 'z') {
            texto[i] = texto[i] - 32;
        }
        i++;
    }
}

// Copia um texto para outro
void copiarTexto(char destino[], char origem[]) {
    int i = 0;
    while (origem[i] != '\0') {
        destino[i] = origem[i];
        i++;
    }
    destino[i] = '\0';
}

// Compara dois textos ignorando maiusculas/minusculas (1 = iguais, 0 = diferentes)
int textosIguais(char a[], char b[]) {
    int i = 0, iguais = 1;
    while (a[i] != '\0' && b[i] != '\0') {
        if (minuscula(a[i]) != minuscula(b[i])) {
            iguais = 0;
        }
        i++;
    }
    if (a[i] != b[i]) {
        iguais = 0;
    }
    return iguais;
}

// Retorna o indice do jogador no vetor pelo ID, ou -1 se nao existir
int indiceJogadorPorId(int id) {
    int i;
    for (i = 0; i < totalJogadores; i++) {
        if (listaJogadores[i].idJogador == id) {
            return i;
        }
    }
    return -1;
}

// Retorna o indice da maquina no vetor pelo ID, ou -1 se nao existir
int indiceMaquinaPorId(int id) {
    int i;
    for (i = 0; i < totalMaquinas; i++) {
        if (catalogoMaquinas[i].idMaquina == id) {
            return i;
        }
    }
    return -1;
}

// Mostra o apelido do jogador (ou "(removido)"), ocupando 15 colunas
void mostrarApelido(int id) {
    int pos = indiceJogadorPorId(id);
    if (pos == -1) {
        printf("%-15s", "(removido)");
    } else {
        printf("%-15s", listaJogadores[pos].apelido);
    }
}

// Mostra o nome do jogo (ou "(removida)"), ocupando 25 colunas
void mostrarNomeMaquina(int id) {
    int pos = indiceMaquinaPorId(id);
    if (pos == -1) {
        printf("%-25s", "(removida)");
    } else {
        printf("%-25s", catalogoMaquinas[pos].nomeJogo);
    }
}

// ----------------------------------------------------------------------------
// NOVAS FUNCOES (Checkpoint 2)
// ----------------------------------------------------------------------------

// Limpa a tela (Windows = cls | Linux/macOS = clear)
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Espera o usuario apertar ENTER (evita que a tela seja limpa antes da leitura)
void pausar() {
    int c;
    printf("\nPressione ENTER para continuar...");
    // 1) descarta o resto da linha digitada antes (inclui o ENTER que sobrou no buffer)
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    // 2) espera um novo ENTER
    if (c != EOF) {
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

// Troca ';' por espaco: o ';' e o separador das colunas do .csv e nao pode aparecer nos textos
void tirarPontoVirgula(char texto[]) {
    int i = 0;
    while (texto[i] != '\0') {
        if (texto[i] == ';') {
            texto[i] = ' ';
        }
        i++;
    }
}

// Le uma linha do .csv ja limpa: remove o BOM do UTF-8 e o ENTER (\r\n) do final.
// Retorna 1 se leu uma linha, 0 se o arquivo acabou
int lerLinhaCSV(FILE *arq, char linha[], int tamanho) {
    int n;
    if (fgets(linha, tamanho, arq) == NULL) {
        return 0;
    }
    if ((unsigned char)linha[0] == 0xEF && (unsigned char)linha[1] == 0xBB &&
        (unsigned char)linha[2] == 0xBF) {
        memmove(linha, linha + 3, strlen(linha + 3) + 1);
    }
    n = strlen(linha);
    while (n > 0 && (linha[n - 1] == '\n' || linha[n - 1] == '\r')) {
        linha[n - 1] = '\0';
        n--;
    }
    return 1;
}

// Troca ',' por '.' (Excel em portugues grava 2,00 em vez de 2.00)
void virgulaParaPonto(char texto[]) {
    int i = 0;
    while (texto[i] != '\0') {
        if (texto[i] == ',') {
            texto[i] = '.';
        }
        i++;
    }
}

#endif
