#include "microfone.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <math.h>

// Prepara o pino para receber os sinais elétricos da placa BitDogLab
void microfone_init(void) {
    adc_init();
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_ADC_CANAL);
}

// Grava o som em um array (buffer) passado por referência
void gravar_audio(uint16_t *buffer) {
    uint32_t intervalo_us = 1000000 / TAXA_AMOSTRAGEM; 

    for (int i = 0; i < TAMANHO_BUFFER; i++) {
        uint64_t inicio = time_us_64(); 
        buffer[i] = adc_read();
        while ((time_us_64() - inicio) < intervalo_us) {
            tight_loop_contents();
        }
    }
}

// COLOQUE A FUNÇÃO AQUI: A inteligência matemática do afinador
float calcular_frequencia(uint16_t *buffer) {
    long soma = 0;
    
    for (int i = 0; i < TAMANHO_BUFFER; i++) {
        soma += buffer[i];
    }
    float media = (float)soma / TAMANHO_BUFFER;

    float sinal[TAMANHO_BUFFER];
    for (int i = 0; i < TAMANHO_BUFFER; i++) {
        sinal[i] = (float)buffer[i] - media;
    }

    float max_correlacao = 0;
    int melhor_lag = 0;
    
    for (int lag = 20; lag < TAMANHO_BUFFER / 2; lag++) {
        float correlacao = 0;
        for (int i = 0; i < TAMANHO_BUFFER - lag; i++) {
            correlacao += sinal[i] * sinal[i + lag];
        }

        if (correlacao > max_correlacao) {
            max_correlacao = correlacao;
            melhor_lag = lag;
        }
    }

    if (melhor_lag > 0 && max_correlacao > 500000.0f) { 
        return (float)TAXA_AMOSTRAGEM / (float)melhor_lag;
    }

    return 0.0f;
}