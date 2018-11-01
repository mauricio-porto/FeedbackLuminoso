/**
 * Dois modos de operação:
 *   1) acende LEDs após batida, ou
 *   2) acende LEDs e aguarda batida, apagando e acendendo outros
 *
 *
 *
 *
 */

#include "Arduino.h"

#define NR_PELES 5

#define MODO_PIN 2

#define TRIGGER_1 A2
#define LEDS_1 2

int triggersPins[] = {A2, A3, A4, A5, A6};
int ledsPins[] = {3, 4, 5, 6, 7};

const int threshold = 100;

int ultimaPele = 0;
boolean aguardando = false;

void setup()
{
	//Serial.begin(115200);

	for (int i=0; i < NR_PELES; i++) {
		pinMode(triggersPins[i], INPUT);
		digitalWrite(ledsPins[i], LOW);
	}
	pinMode(MODO_PIN, INPUT_PULLUP);
}

void loop()
{
	if(digitalRead(MODO_PIN)) {			// Modo 1 - acende após batida
		// Sempre apaga todos antes
		for (int led=0; led < NR_PELES; led++) {
			digitalWrite(ledsPins[led], LOW);
		}
		// Lê cada um dos triggers e caso ativado, acende LEDs correspondentes por uma fração de segundo (PISCA)
		int signal = 0;
		for (int pele=0; pele < NR_PELES; pele++) {
			signal = analogRead(triggersPins[pele]);
			if (signal > threshold) {
				digitalWrite(ledsPins[pele], HIGH);
				delay(100);
				digitalWrite(ledsPins[pele], LOW);
				break;
			}
		}
	} else {												// Modo 2 - modo siga-me
		// Acende aleatoriamente os LEDs de uma pele e aguarda batida
		// Quando a pele for batida, apaga
		int pele = 0;
		if (!aguardando) {
			do {
				pele = random(NR_PELES);
			} while (pele == ultimaPele);
			ultimaPele = pele;
			digitalWrite(ledsPins[pele], HIGH);
			aguardando = true;
			goto bailOut;
		}
		int signal = 0;
		signal = analogRead(triggersPins[ultimaPele]);
		if (signal > threshold) {
			aguardando = false;
			digitalWrite(ledsPins[ultimaPele], LOW);
		}
	}
bailOut:
	delay(100);
}
