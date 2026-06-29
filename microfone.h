#ifndef MICROFONE_H
#define MICROFONE_H

#include <stdint.h>

#define TAMANHO_BUFFER 1024
#define TAXA_AMOSTRAGEM 10000.0f
#define MIC_PIN 28
#define MIC_CANAL 2

void microfone_init(void);
void capturar_audio(uint16_t *buffer);
float calcular_frequencia(uint16_t *buffer);

#endif // MICROFONE_H