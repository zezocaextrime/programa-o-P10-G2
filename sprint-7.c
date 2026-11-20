#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEITURAS 100
#define MAX_LINHA 128

typedef struct {
    char id[20];
    double ganho;
    double desvio;
    double temp_min;
    double temp_max;
} Sensor;

typedef struct {
    Sensor sensor;
    int instante;
    int valor;
    double temp;
} Leitura;

const Sensor SENSORES[] = {
    {"TEMP1", 260.0, -20.0, -10.0, 190.0},
    {"TEMP2", 250.0, -15.0,   0.0, 150.0},
    {"TEMP3", 240.0, -10.0, -10.0, 100.0}
};
const int NUM_SENSORES = sizeof(SENSORES) / sizeof(SENSORES[0]);

int procurar_sensor(const char *id, const Sensor sensores[], int num_sensores, Sensor *encontrado) {
    for (int i = 0; i < num_sensores; i++) {
        if (strcmp(sensores[i].id, id) == 0) {
            *encontrado = sensores[i];
            return 1;
        }
    }
    return 0;
}

double converter_temperatura(Sensor s, int valor) {
    return s.ganho * valor / 1023.0 + s.desvio;
}

int validar_gama(Sensor s, double temp) {
    return (temp >= s.temp_min && temp <= s.temp_max);
}

int parse_linha(const char *linha, char *id, int *instante, int *valor) {
    char inst_str[32], val_str[32], extra[32];
    int campos = sscanf(linha, "%19s %31s %31s %31s", id, inst_str, val_str, extra);

    if (campos < 1) {
        return 0;
    }

    Sensor temp_s;
    if (!procurar_sensor(id, SENSORES, NUM_SENSORES, &temp_s)) {
        return -1;
    }

    if (campos != 3) {
        return 0;
    }

    char *endptr;
    long inst_l = strtol(inst_str, &endptr, 10);
    if (*endptr != '\0') {
        return 0;
    }

    long val_l = strtol(val_str, &endptr, 10);
    if (*endptr != '\0') {
        return 0;
    }

    *instante = (int)inst_l;
    *valor = (int)val_l;
    return 1;
}

void imprimir_leituras(const Leitura leituras[], int total) {
    printf("\nLeituras registadas (sensor instante: valor -> temperatura):\n");
    for (int i = 0; i < total; i++) {
        printf("%s %d: %3d -> %6.2f\n",
               leituras[i].sensor.id,
               leituras[i].instante,
               leituras[i].valor,
               leituras[i].temp);
    }
}

void imprimir_estatisticas(const Leitura leituras[], int total, const Sensor sensores[], int num_sensores) {
    for (int s = 0; s < num_sensores; s++) {
        int count = 0;
        double min = 0.0, max = 0.0, soma = 0.0;

        for (int i = 0; i < total; i++) {
            if (strcmp(leituras[i].sensor.id, sensores[s].id) == 0) {
                double t = leituras[i].temp;
                if (count == 0) {
                    min = max = t;
                } else {
                    if (t < min) min = t;
                    if (t > max) max = t;
                }
                soma += t;
                count++;
            }
        }

        if (count > 0) {
            double media = soma / count;
            double soma_quad = 0.0;

            for (int i = 0; i < total; i++) {
                if (strcmp(leituras[i].sensor.id, sensores[s].id) == 0) {
                    double diff = leituras[i].temp - media;
                    soma_quad += diff * diff;
                }
            }
            double desvio = sqrt(soma_quad / count);

            printf("\nSensor %s\n", sensores[s].id);
            printf("Número de valores lidos: %d\n", count);
            printf("Temperatura mínima: %8.2f\n", min);
            printf("Temperatura máxima: %8.2f\n", max);
            printf("Temperatura média:  %8.2f\n", media);
            printf("Desvio-padrão:      %8.2f\n", desvio);
        }
    }
}

int main(void) {
    Leitura leituras[MAX_LEITURAS];
    int total_leituras = 0;
    char linha[MAX_LINHA];

    printf("Introduza as leituras ou FIM para terminar:\n");

    while (total_leituras < MAX_LEITURAS && fgets(linha, sizeof(linha), stdin) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (strcmp(linha, "FIM") == 0) {
            break;
        }

        char id[20];
        int instante, valor;
        int estado = parse_linha(linha, id, &instante, &valor);

        if (estado == -1) {
            printf("Sensor desconhecido\n");
            continue;
        }
        if (estado == 0) {
            printf("Linha inválida\n");
            continue;
        }

        Sensor sensor_atual;
        procurar_sensor(id, SENSORES, NUM_SENSORES, &sensor_atual);
        double temp = converter_temperatura(sensor_atual, valor);

        if (!validar_gama(sensor_atual, temp)) {
            printf("Valor fora da gama\n");
            continue;
        }

        printf("%.2f\n", temp);
        leituras[total_leituras].sensor = sensor_atual;
        leituras[total_leituras].instante = instante;
        leituras[total_leituras].valor = valor;
        leituras[total_leituras].temp = temp;
        total_leituras++;
    }

    imprimir_leituras(leituras, total_leituras);
    imprimir_estatisticas(leituras, total_leituras, SENSORES, NUM_SENSORES);

    return 0;
}
