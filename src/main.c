#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_LINE_LENGTH 204800
#define MAX_STUDENTS 10
#define MAX_COLUMNS 190
#define TAM_INSCRICAO 13

typedef struct {
    char inscricao[TAM_INSCRICAO];
    float nota_CN;
    float nota_CH;
    float nota_LC;
    float nota_MT;
    float nota_RED;
} Student;

int obterIndiceDaColuna(const char *nomeColuna, const char *nomesColunas[], int numColunas) {
    for (int i = 0; i < numColunas; i++) {
        if (strcmp(nomeColuna, nomesColunas[i]) == 0) {
            return i;
        }
    }
    return -1;
}

double calcularMedia(double lista[], int tamanho) {
    double soma = 0.0;
    for (int i = 0; i < tamanho; i++) {
        soma += lista[i];
    }
    return soma / tamanho;
}

double calcularCorrelacaoPearson(double lista1[], double lista2[], int tamanho) {
    double media1 = calcularMedia(lista1, tamanho);
    double media2 = calcularMedia(lista2, tamanho);

    double numerador = 0.0;
    double denominador1 = 0.0;
    double denominador2 = 0.0;

    for (int i = 0; i < tamanho; i++) {
        numerador += (lista1[i] - media1) * (lista2[i] - media2);
        denominador1 += pow((lista1[i] - media1), 2);
        denominador2 += pow((lista2[i] - media2), 2);
    }

    // Evitar divisão por zero
    if (denominador1 == 0 || denominador2 == 0) {
        return 0.0;
    }

    return numerador / (sqrt(denominador1) * sqrt(denominador2));
}

int obterNomesDasColunas(FILE *file, const char *nomesColunas[]) {
    char line[MAX_LINE_LENGTH];
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        fprintf(stderr, "Erro ao ler o cabeçalho do arquivo CSV.\n");
        exit(EXIT_FAILURE);
    }

    int numColunas = 0;
    char *token = strtok(line, ",");
    while (token != NULL && numColunas < MAX_COLUMNS) {
        nomesColunas[numColunas++] = strdup(token);
        token = strtok(NULL, ",");
    }

    // Imprimir nomes das colunas e seus índices
    for (int i = 0; i < numColunas; i++) {
        printf("Coluna %d: %s\n", i, nomesColunas[i]);
    }

    return numColunas;
}

void preencherCamposVaziosComZero(char *linha) {
    char *delim = ",";
    char novaLinha[strlen(linha) * 2];  // Uma estimativa segura do tamanho necessário
    char *ptr_original = linha;
    char *ptr_novaLinha = novaLinha;

    // Inicializa a nova linha como uma string vazia
    novaLinha[0] = '\0';

    // Flag para verificar se o último caractere foi uma vírgula
    int ultimaVirgula = 0;

    // Processa cada caractere da linha original
    while (*ptr_original != '\0') {
        if (*ptr_original == ',') {
            if (ultimaVirgula) {
                // Adiciona zero apenas se a vírgula for precedida por outra vírgula
                strcat(ptr_novaLinha, "0");
            }
            strcat(ptr_novaLinha, delim);
            ultimaVirgula = 1;
        } else {
            // Copia o caractere normalmente
            strncat(ptr_novaLinha, ptr_original, 1);
            ultimaVirgula = 0;
        }

        ptr_original++;
        ptr_novaLinha++;
    }

    // Copia a nova linha de volta para a linha original
    strcpy(linha, novaLinha);
}


int lerBaseDeDados(Student *students, const char *filename, int n, const char *nomesColunas[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    int numColunas = obterNomesDasColunas(file, nomesColunas);
    printf("Num colunas: %d\n", numColunas);
    int indiceInscricao = obterIndiceDaColuna("NU_INSCRICAO", nomesColunas, numColunas);
    int indiceNotaCN = obterIndiceDaColuna("NU_NOTA_CN", nomesColunas, numColunas);
    int indiceNotaCH = obterIndiceDaColuna("NU_NOTA_CH", nomesColunas, numColunas);
    int indiceNotaLC = obterIndiceDaColuna("NU_NOTA_LC", nomesColunas, numColunas);
    int indiceNotaMT = obterIndiceDaColuna("NU_NOTA_MT", nomesColunas, numColunas);
    int indiceNotaRED = obterIndiceDaColuna("NU_NOTA_REDACAO", nomesColunas, numColunas);
    printf("indice Nota CN: %d", indiceNotaCN);

    int linha_atual = 0;
    char linha[MAX_LINE_LENGTH];

    // Ignorar a primeira linha (cabeçalho)
    fgets(linha, MAX_LINE_LENGTH, file);

    while (fgets(linha, MAX_LINE_LENGTH, file) != NULL && linha_atual < n) {

        printf("\n\n LINHA ANTES: %s\n\n", linha);
        preencherCamposVaziosComZero(linha);
        printf("\n\n LINHA DEPOIS: %s\n\n", linha);
        char *token = strtok(linha, ",");
        int coluna_atual = 0;

        while (token != NULL && (coluna_atual < numColunas)) {
            float aux_nota_CN = 0;
            float aux_nota_CH = 0;
            float aux_nota_LC = 0;
            float aux_nota_MT = 0;
            float aux_nota_RED = 0;
            //char aux_insc[12];
            //printf("\nColuna atual: %d", coluna_atual);

            if (coluna_atual == indiceNotaCN) {
                printf("\nCN: %s", token);
                aux_nota_CN = atof(token);
                students[linha_atual].nota_CN = aux_nota_CN;
            } else if (coluna_atual == indiceNotaCH) {
                aux_nota_CH = atof(token);
                students[linha_atual].nota_CH = aux_nota_CH;
                printf("\nCH: %s", token);
            } else if (coluna_atual == indiceNotaLC) {
                aux_nota_LC = atof(token);
                students[linha_atual].nota_LC = aux_nota_LC;
                printf("\nLC: %s", token);
            } else if (coluna_atual == indiceNotaMT) {
                aux_nota_MT = atof(token);
                students[linha_atual].nota_MT = aux_nota_MT;
                printf("\nMT: %s", token);
            } else if (coluna_atual == indiceNotaRED) {
                aux_nota_RED = atof(token);
                students[linha_atual].nota_RED = aux_nota_RED;
                printf("\nRED: %s", token);
            } else if (coluna_atual == indiceInscricao) {
                printf("\nINSC: %s", token);
                strncpy(students[linha_atual].inscricao, token, TAM_INSCRICAO-1);
                students[linha_atual].inscricao[TAM_INSCRICAO-1] = '\0';
            }

            token = strtok(NULL, ",");
            coluna_atual++;
        }

        printf("\nNUMERO INSCRICAO ARMAZENADO: %s", students[linha_atual].inscricao);
        printf("\nLINHA ATUAL: %d\n\n", linha_atual);

        linha_atual++;
    }

    fclose(file);

    return linha_atual;
}


void liberarNomesColunas(const char *nomesColunas[], int numColunas) {
    for (int i = 0; i < numColunas; i++) {
        free((void *)nomesColunas[i]); // Casting para void* é necessário
    }
}

int main() {
    srand(time(NULL));

    const char *filename = "enem_data/microdados_enem_2016_coma.csv";

    const char *nomesColunas[MAX_COLUMNS];
    int numColunas = 0;

    Student students[MAX_STUDENTS];

    int numEstudantes = lerBaseDeDados(students, filename, MAX_STUDENTS, nomesColunas);

    // Imprimir as notas_CN dos estudantes
// Imprimir as notas_CN dos estudantes
    for (int i = 0; i < numEstudantes; i++) {
        printf("Estudante %s - Nota CN: %.2f\n", students[i].inscricao, students[i].nota_CN);
   }

    liberarNomesColunas(nomesColunas, numColunas);

    return 0;
}
