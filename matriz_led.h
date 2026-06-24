#ifndef MATRIZ_LED_H
#define MATRIZ_LED_H

#include <stdint.h>

#define LED_PIN 7
#define NUM_LEDS 25 // 5x5

void matriz_init(void);
void matriz_clear(void);
void matriz_update(void);
void atualizar_agulha_led(float freq_lida, float freq_alvo);

#endif