#include "microfone.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

uint dma_canal;

void microfone_init(void) {
    adc_init();
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_CANAL);
    
    // Configura o ADC para capturar continuamente (Free-running)
    adc_fifo_setup(
        true,    // Habilita FIFO
        true,    // Habilita request de DMA
        1,       // Threshold do DREQ
        false,   // Sem bit de erro
        false    // Shift para 8 bits falso (mantém 12 bits)
    );
    
    // Configura o clock do ADC para a taxa de amostragem desejada
    adc_set_clkdiv(48000000.0f / TAXA_AMOSTRAGEM);

    // Configuração do DMA para transferir do ADC para a memória
    dma_canal = dma_claim_unused_channel(true);
    dma_channel_config config = dma_channel_get_default_config(dma_canal);
    
    channel_config_set_transfer_data_size(&config, DMA_SIZE_16);
    channel_config_set_read_increment(&config, false);
    channel_config_set_write_increment(&config, true);
    channel_config_set_dreq(&config, DREQ_ADC);  

    dma_channel_configure(
        dma_canal,
        &config,
        NULL,
        &adc_hw->fifo, 
        TAMANHO_BUFFER,
        false 
    );
}

void capturar_audio(uint16_t *buffer) {
    adc_fifo_drain();
    adc_run(false);
    dma_channel_set_write_addr(dma_canal, buffer, true);
    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_canal);
    adc_run(false);
}

float calcular_frequencia(uint16_t *buffer) {
    float max_correlacao = 0;
    int melhor_lag = 0;
    int lag_min = 25;
    int lag_max = 500;

    // Cálculo da autocorrelação
    for (int lag = lag_min; lag <= lag_max; lag++) {
        float correlacao = 0;
        for (int i = 0; i < TAMANHO_BUFFER - lag; i++) {
            int amostra1 = buffer[i] - 2048;
            int amostra2 = buffer[i + lag] - 2048;
            correlacao += amostra1 * amostra2;
        }
        if (correlacao > max_correlacao) {
            max_correlacao = correlacao;
            melhor_lag = lag;
        }
    }

    if (melhor_lag == 0) return 0.0f;

    // Interpolação parabólica para refinar o pico
    float lag_interpolado = (float)melhor_lag;
    if (melhor_lag > lag_min && melhor_lag < lag_max) {
        float y1 = 0, y2 = 0, y3 = 0;
        
        for (int i = 0; i < TAMANHO_BUFFER - (melhor_lag - 1); i++) {
            y1 += (buffer[i] - 2048) * (buffer[i + melhor_lag - 1] - 2048);
        }
        for (int i = 0; i < TAMANHO_BUFFER - melhor_lag; i++) {
            y2 += (buffer[i] - 2048) * (buffer[i + melhor_lag] - 2048);
        }
        for (int i = 0; i < TAMANHO_BUFFER - (melhor_lag + 1); i++) {
            y3 += (buffer[i] - 2048) * (buffer[i + melhor_lag + 1] - 2048);
        }

        float alfa = y1;
        float beta = y2;
        float gama = y3;
        float denominador = 2.0f * (alfa - 2.0f * beta + gama);

        if (denominador != 0.0f) {
            lag_interpolado += (alfa - gama) / denominador;
        }
    }

    return TAXA_AMOSTRAGEM / lag_interpolado;
}