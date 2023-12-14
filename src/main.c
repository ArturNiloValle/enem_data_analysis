#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_LINE_LENGTH 102400
#define MAX_STUDENTS 1000
#define MAX_COLUMNS 180

typedef struct {
    float nota;

} Student;


int obterIndiceDaColuna(const char *nomeColuna, const char *nomesColunas[], int numColunas) {
    for (int i = 0; i < numColunas; i++) {
        //printf("\ncoluna passada: %s\n",nomeColuna);
        //printf("coluna comparada: %s\n", nomesColunas[i]);
        if (strcmp(nomeColuna, nomesColunas[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int obterNomesDasColunas(FILE *file, const char *nomesColunas[]) {
    char line[MAX_LINE_LENGTH];
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        fprintf(stderr, "Erro ao ler o cabeçalho do arquivo CSV.\n");
        exit(EXIT_FAILURE);
    }

    printf("Cabeçalho lido: %s\n", line);  // Mensagem de depuração

    int numColunas = 0;
    char *token = strtok(line, ",");
    while (token != NULL && numColunas < MAX_COLUMNS) {
        printf("Nome da coluna: %s\n", token);  // Mensagem de depuração
        nomesColunas[numColunas++] = strdup(token);
        token = strtok(NULL, ",");
    }

    return numColunas;
}


int lerBaseDeDados(Student *students, const char *filename, int n, const char *nomesColunas[], int numColunas) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    numColunas = obterNomesDasColunas(file, nomesColunas);
    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && count < n) {
        for (int i = 0; i < numColunas; i++) {
            sscanf(line, "%f,", &students[count].notas[i]);
        }

        // Preencha outras informações conforme necessário, como situacao

        count++;
    }

    fclose(file);

    return numColunas;
}


void liberarNomesColunas(const char *nomesColunas[], int numColunas) {
    for (int i = 0; i < numColunas; i++) {
        free((char *)nomesColunas[i]);
    }
}

int main() {
    srand(time(NULL));

    const char *filename = "enem_data/microdados_enem_2016_coma.csv";

    const char *nomesColunas[MAX_COLUMNS];
    int numColunas = 0;

    Student students[MAX_STUDENTS];

    numColunas = lerBaseDeDados(students, filename, MAX_STUDENTS, nomesColunas, numColunas);

    // Escolha aleatória de dois fatores para calcular o coeficiente de Pearson
    const char *fator1 = "NU_NOTA_CN";  // Substitua pelos nomes reais das colunas
    const char *fator2 = "NU_NOTA_CH";
    int indice1 = obterIndiceDaColuna(fator1, nomesColunas, numColunas);
    int indice2 = obterIndiceDaColuna(fator2, nomesColunas, numColunas);

    if (indice1 == -1 || indice2 == -1) {
        printf("Erro: Nome da coluna não encontrado.\n");
        liberarNomesColunas(nomesColunas, numColunas);
        return 1;
    }



    //float pearson = calcularPearson(students, MAX_STUDENTS, indice1, indice2);

    //printf("Coeficiente de Pearson: %.4f\n", pearson);

    liberarNomesColunas(nomesColunas, numColunas);

    return 0;
}
