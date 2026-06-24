#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"

// Incluímos o nosso próprio cardápio!
#include "botoes.h" 

// Definições I2C (preparando para o OLED)
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Variáveis do ADC
#define TAMANHO_BUFFER 1024
uint16_t buffer_amostras[TAMANHO_BUFFER];
volatile int indice_amostra = 0;
volatile bool buffer_cheio = false;

bool timer_amostragem_callback(struct repeating_timer *t) {
    if (!buffer_cheio) {
        buffer_amostras[indice_amostra++] = adc_read();
        if (indice_amostra >= TAMANHO_BUFFER) {
            buffer_cheio = true; 
            indice_amostra = 0;  
        }
    }
    return true;
}

int main() {
    stdio_init_all();

    // Inicia o módulo dos botões com apenas 1 linha!
    inicializar_botoes(); 

    // Inicializa o I2C (Display no futuro)
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o ADC e Timer
    adc_init();
    adc_gpio_init(28);
    adc_select_input(2);

    struct repeating_timer timer;
    add_repeating_timer_us(-125, timer_amostragem_callback, NULL, &timer);

    printf("RP TUNER INICIADO!\n");

    while (true) {
        if (buffer_cheio) {
            // Processamento do áudio virá aqui
            buffer_cheio = false; 
        }
        sleep_ms(10);
    }
}