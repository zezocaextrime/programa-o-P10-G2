#include <stdio.h>

int main(void) {
	int x;

	scanf("%d", &x);
	double temp = 260.0 * x / 1023.0 - 20.0;
	printf("%.2f °C\n", temp);

	return 0
    ;
}
