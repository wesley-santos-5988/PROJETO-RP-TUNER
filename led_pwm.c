#include "led_pwm.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <math.h>

#define LED_VERDE_PIN 11 // Pino do LED verde na BitDogLab

uint slice_num;

void pwm_init_led(void) {
    // Configura o pino para a função PWM
    gpio_set_function(LED_VERDE_PIN, GPIO_FUNC_PWM);
    
    // Descobre qual o "slice" (canal interno) do PWM que controla este pino
    slice_num = pwm_gpio_to_slice_num(LED_VERDE_PIN);

    // Configura o PWM para um ciclo de resolução de 16 bits (0 a 65535)
    pwm_set_wrap(slice_num, 65535);
    
    // Inicia com o LED apagado (nível 0)
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_VERDE_PIN), 0);
    
    // Habilita o hardware de PWM
    pwm_set_enabled(slice_num, true);
}

void pwm_atualizar_brilho(float freq_lida, float freq_alvo) {
    // Calcula a diferença entre a afinação real e o alvo
    float diferenca = fabs(freq_lida - freq_alvo);
    
    uint16_t brilho = 0;

    // Se estiver a menos de 5 Hz de diferença, começa a acender
    if (diferenca <= 5.0f) {
        // Quanto menor a diferença, maior o brilho. Cravar = 65535 (100%)
        float intensidade = (5.0f - diferenca) / 5.0f; 
        brilho = (uint16_t)(intensidade * 65535);
    } else {
        brilho = 0; // Desafinado (muito longe do alvo): LED apagado
    }

    // Aplica o brilho no hardware
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(LED_VERDE_PIN), brilho);
}