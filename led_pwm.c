#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

// Na BitDogLab, os LEDs RGB comuns ficam nos pinos 11 (Azul), 12 (Verde) e 13 (Vermelho).
// Escolha um deles para responder ao PWM (ex: Pino 12 - Verde)
#define LED_PWM_PIN 12 

// Inicializa o pino escolhido para funcionar como PWM
void pwm_init_led(void) {
    // Configura o pino para a função de PWM
    gpio_set_function(LED_PWM_PIN, GPIO_FUNC_PWM);
    
    // Descobre qual slice de hardware do RP2040 controla esse pino
    uint slice_num = pwm_gpio_to_slice_num(LED_PWM_PIN);
    
    // Configuração do período (Wrap): 255 define uma resolução de 8 bits (0 a 255)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 255);
    
    // Inicializa o hardware com a configuração e ativa o PWM
    pwm_init(slice_num, &config, true);
}

// Atualiza o brilho do LED alterando o ciclo de trabalho (Duty Cycle)
void pwm_atualizar_brilho(int brilho) {
    // Garante que o brilho fique dentro do limite de 0 a 255
    if (brilho < 0) brilho = 0;
    if (brilho > 255) brilho = 255;

    uint slice_num = pwm_gpio_to_slice_num(LED_PWM_PIN);
    uint channel = pwm_gpio_to_channel(LED_PWM_PIN);
    
    // Define o nível do canal para alterar a intensidade luminosa
    pwm_set_chan_level(slice_num, channel, brilho);
}