#include <stdio.h>
#include <math.h> // Essencial para gerarmos a onda do "violão virtual"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"

#include "botoes.h" 
#include "display.h"
#include "matriz_led.h"
#include "microfone.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

#define PI 3.14159265358979323846

int main() {
    stdio_init_all();
    
    // O nosso balde de áudio
    uint16_t buffer_mic[TAMANHO_BUFFER];

    // Inicializações dos nossos módulos (cada um cuida da sua própria configuração de hardware)
    inicializar_botoes();
    matriz_init();
    microfone_init(); // Isso já resolve o adc_init e os pinos do microfone internamente!

    // Inicialização do I2C para o Display
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o Ecrã OLED
    oled_init();
    atualizar_ecra_afinacao(0.0);

// ... inicializações anteriores mantidas ...

    // O ÚNICO LOOP INFINITO (Versão Placa Física)
    while (true) {
        // 1. Ouve o mundo real: Captura o som do microfone da BitDogLab
        gravar_audio(buffer_mic);

        // 2. A HORA DA VERDADE: O algoritmo descobre a frequência da nota tocada
        float freq_calculada = calcular_frequencia(buffer_mic);

        // 3. Pegamos a frequência alvo da corda que está selecionada pelos botões
        float alvo = violao[corda_atual].freq_alvo;

        // 4. Atualizamos os visores apenas se o microfone captar algum som audível
        if (freq_calculada > 0.0f) {
            atualizar_ecra_afinacao(freq_calculada);
            atualizar_agulha_led(freq_calculada, alvo);
        }

        // Uma pausa leve para o processador respirar
        sleep_ms(10);
    }
}