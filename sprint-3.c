#include <stdio.h>

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
    int valor_sensor;
    double temperatura;

    while (1) {
        valor_sensor = ler_inteiro();

        if (valor_sensor == -1) {
            break;
        }

        temperatura = converter_sensor_para_temp(valor_sensor);

        if (temperatura >= -10.0 && temperatura <= 190.0) {
            printf("%.2f ºC\n\n", temperatura);
        } else {
            printf("Valor fora da gama\n\n");
        }
    }

    return 0;
}
