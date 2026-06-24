#ifndef MICROFONE_H
#define MICROFONE_H

#include <stdint.h>

// Frequência de amostragem (quantas leituras por segundo)
// 4000 Hz é um valor excelente para as frequências graves do violão.
#define TAXA_AMOSTRAGEM 4000 

// Quantidade de amostras que vamos gravar antes de processar.
// 1024 amostras a 4000Hz = captura cerca de 0.25 segundos de áudio por vez.
#define TAMANHO_BUFFER 1024

// Pinos na placa BitDogLab
#define MIC_PIN 28
#define MIC_ADC_CANAL 2

void microfone_init(void);
void gravar_audio(uint16_t *buffer);
float calcular_frequencia(uint16_t *buffer); // Apenas isso!

#endif