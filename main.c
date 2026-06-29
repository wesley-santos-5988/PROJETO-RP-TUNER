#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"

#include "botoes.h" 
#include "display.h"
#include "matriz_led.h"
#include "microfone.h"
#include "led_pwm.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Variável global de controlo para o temporizador
volatile bool realizar_leitura = false;

// Função de Callback do Temporizador (disparada automaticamente pelo hardware)
bool timer_callback(struct repeating_timer *t) {
    realizar_leitura = true; // Avisa o loop principal que é hora de ler o áudio
    return true; // Retorna true para que o temporizador continue a repetir-se
}

int main() {
    stdio_init_all();
    
    uint16_t buffer_audio[TAMANHO_BUFFER];

    inicializar_botoes();
    matriz_init();
    microfone_init(); 
    pwm_init_led();

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    oled_init();
    atualizar_ecra_afinacao(0.0);

    struct repeating_timer timer;
    add_repeating_timer_ms(150, timer_callback, NULL, &timer);

    while (true) {
        // O processador fica livre aqui até que o temporizador mude a flag
        if (realizar_leitura) {
            realizar_leitura = false; // Reinicia a flag para a próxima leitura
            
            // Captura o áudio real via microfone e DMA
            capturar_audio(buffer_audio);

            float freq_calculada = calcular_frequencia(buffer_audio);
            float alvo = violao[corda_atual].freq_alvo;

            if (freq_calculada > 0.0f) {
                atualizar_ecra_afinacao(freq_calculada);
               atualizar_agulha_led(freq_calculada, alvo);
               pwm_atualizar_brilho(freq_calculada, alvo); // Ajusta o brilho do LED
            } else {
                pwm_atualizar_brilho(0.0f, alvo); // Desliga o LED se houver silêncio
            }
        }
    }

    return 0;
}