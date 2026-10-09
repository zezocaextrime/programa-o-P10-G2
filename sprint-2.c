#include <stdio.h>

int main(void) {
	int x;

	while (1) {
		printf("Introduza um valor do sensor: ");
		if (scanf("%d", &x) == 1) {
			break;
		}

		printf("Valor rejeitado\n");
		int c;
		while ((c = getchar()) != '\n' && c != EOF) {
		}
	}

	double temp = 260.0 * x / 1023.0 - 20.0;
	if (temp > -10.0 && temp < 190.0) {
		printf("%.2f °C\n", temp);
	} else {
		printf("Valor fora da gama\n");
	}

	return 0;
}
