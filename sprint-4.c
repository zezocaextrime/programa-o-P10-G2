#include <stdio.h>
#include <math.h>

#define MAX_LEITURAS 20

int ler_inteiro(void) {
    int valor;
    int resultado;

    while (1) {
        printf("Introduza o valor do sensor (ou -1 para terminar): ");
        resultado = scanf("%d", &valor);

        if (resultado == 1) {
            return valor;
        }

        while (getchar() != '\n');
        printf("Entrada invalida! Por favor introduza um numero inteiro.\n");
    }
}

double converter_sensor_para_temp(int x) {
    return (260.0 * x / 1023.0) - 20.0;
}

int main(void) {
    int sensores[MAX_LEITURAS];
    double temperaturas[MAX_LEITURAS];
    int total_lidos = 0;

    int valor_sensor;
    double temperatura;

    while (total_lidos < MAX_LEITURAS) {
        valor_sensor = ler_inteiro();

        if (valor_sensor == -1) {
            break;
        }

        temperatura = converter_sensor_para_temp(valor_sensor);

        if (temperatura >= -10.0 && temperatura <= 190.0) {
            printf("%.2f ºC\n\n", temperatura);
            sensores[total_lidos] = valor_sensor;
            temperaturas[total_lidos] = temperatura;
            total_lidos++;
        } else {
            printf("Valor fora da gama\n\n");
        }
    }

    if (total_lidos > 0) {
        printf("\nValores lidos (sensor -> temperatura):\n");
        for (int i = 0; i < total_lidos; i++) {
            printf("%d -> %.2f ºC\n", sensores[i], temperaturas[i]);
        }

        double min = temperaturas[0];
        double max = temperaturas[0];
        double soma = 0.0;

        for (int i = 0; i < total_lidos; i++) {
            if (temperaturas[i] < min) min = temperaturas[i];
            if (temperaturas[i] > max) max = temperaturas[i];
            soma += temperaturas[i];
        }

        double media = soma / total_lidos;

        double soma_quadrados = 0.0;
        for (int i = 0; i < total_lidos; i++) {
            soma_quadrados += (temperaturas[i] - media) * (temperaturas[i] - media);
        }

        double desvio_padrao = 0.0;
        if (total_lidos > 1) {
            desvio_padrao = sqrt(soma_quadrados / (total_lidos - 1));
        }

        printf("\nNúmero de valores lidos: %d\n", total_lidos);
        printf("Temperatura mínima: %8.2f ºC\n", min);
        printf("Temperatura máxima: %8.2f ºC\n", max);
        printf("Temperatura média:  %8.2f ºC\n", media);
        printf("Desvio-padrão:      %8.2f\n", desvio_padrao);
    }

    return 0;
}
