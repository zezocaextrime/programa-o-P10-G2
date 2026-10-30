#include <stdio.h>
#include <math.h>
#include <assert.h>

#define MAX_LEITURAS 20
#define TEMP_MIN -10.0
#define TEMP_MAX 190.0

/* Converte o valor do sensor numa temperatura. */
double converter_sensor_para_temp(int valor_sensor) {
    return (260.0 * valor_sensor / 1023.0) - 20.0;
}

/* Verifica se a temperatura esta na gama valida. */
int temperatura_valida(double temperatura) {
    return temperatura >= TEMP_MIN && temperatura <= TEMP_MAX;
}

/* Le um numero inteiro do teclado. */
int ler_inteiro(void) {
    int valor, resultado, caractere;

    while (1) {
        printf("Introduza o valor do sensor (ou -1 para terminar): ");
        resultado = scanf("%d", &valor);

        if (resultado == 1) {
            return valor;
        }

        if (resultado == EOF) {
            return -1;
        }

        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }

        printf("Entrada invalida! Introduza um numero inteiro.\n");
    }
}

/* Le os sensores, converte e guarda as temperaturas validas.
   Devolve o numero de leituras guardadas. */
int ler_valores(int sensores[], double temperaturas[], int limite) {
    int total_lidos = 0;

    while (total_lidos < limite) {
        int valor_sensor = ler_inteiro();

        if (valor_sensor == -1) {
            break;
        }

        double temperatura = converter_sensor_para_temp(valor_sensor);

        if (!temperatura_valida(temperatura)) {
            printf("Valor fora da gama\n\n");
            continue;
        }

        sensores[total_lidos] = valor_sensor;
        temperaturas[total_lidos] = temperatura;

        total_lidos++;

        printf("%.2f\n", temperatura);
    }

    return total_lidos;
}

/* Calcula e devolve a temperatura minima. */
double calcular_minimo(const double temperaturas[], int quantidade) {
    double minimo = temperaturas[0];

    for (int i = 1; i < quantidade; i++) {
        if (temperaturas[i] < minimo) {
            minimo = temperaturas[i];
        }
    }

    return minimo;
}

/* Calcula e devolve a temperatura maxima. */
double calcular_maximo(const double temperaturas[], int quantidade) {
    double maximo = temperaturas[0];

    for (int i = 1; i < quantidade; i++) {
        if (temperaturas[i] > maximo) {
            maximo = temperaturas[i];
        }
    }

    return maximo;
}

/* Calcula e devolve a media das temperaturas. */
double calcular_media(const double temperaturas[], int quantidade) {
    double soma = 0.0;

    for (int i = 0; i < quantidade; i++) {
        soma += temperaturas[i];
    }

    return soma / quantidade;
}

/* Calcula e devolve o desvio-padrao populacional. */
double calcular_desvio_padrao(const double temperaturas[], int quantidade) {
    double media = calcular_media(temperaturas, quantidade);
    double soma_quadrados = 0.0;

    for (int i = 0; i < quantidade; i++) {
        double diferenca = temperaturas[i] - media;
        soma_quadrados += diferenca * diferenca;
    }

    return sqrt(soma_quadrados / quantidade);
}

/* Apresenta os valores lidos e as estatisticas. */
void apresentar_resultados(const int sensores[],
                         const double temperaturas[],
                         int quantidade) {

    printf("\nValores lidos (sensor -> temperatura):\n");

    for (int i = 0; i < quantidade; i++) {
        printf("%d -> %6.2f\n", sensores[i], temperaturas[i]);
    }

    printf("\nNumero de valores lidos: %d\n", quantidade);

    if (quantidade == 0) {
        printf("Nao existem temperaturas para calcular estatisticas.\n");
        return;
    }

    printf("Temperatura minima: %9.2f\n",
           calcular_minimo(temperaturas, quantidade));

    printf("Temperatura maxima: %9.2f\n",
           calcular_maximo(temperaturas, quantidade));

    printf("Temperatura media:  %9.2f\n",
           calcular_media(temperaturas, quantidade));

    printf("Desvio-padrao:      %9.2f\n",
           calcular_desvio_padrao(temperaturas, quantidade));
}

#ifdef TESTES

/* Testa as funcoes com valores conhecidos. */
void executar_testes(void) {
    const double temperaturas[] = {10.0, 20.0, 30.0};

    assert(fabs(converter_sensor_para_temp(400) - 81.66) < 0.01);
    assert(fabs(converter_sensor_para_temp(523) - 112.92) < 0.01);

    assert(temperatura_valida(100.0) == 1);
    assert(temperatura_valida(200.0) == 0);

    assert(calcular_minimo(temperaturas, 3) == 10.0);
    assert(calcular_maximo(temperaturas, 3) == 30.0);
    assert(calcular_media(temperaturas, 3) == 20.0);

    assert(fabs(calcular_desvio_padrao(temperaturas, 3) - 8.16497) < 0.00001);

    printf("Todos os testes passaram.\n");
}
#endif

int main(void) {
#ifdef TESTES
    executar_testes();
#else
    int sensores[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];

    int total_lidos = ler_valores(sensores, temperaturas, MAX_LEITURAS);

    apresentar_resultados(sensores, temperaturas, total_lidos);
#endif

    return 0;
}
