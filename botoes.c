#include "botoes.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

// Aqui nós criamos as variáveis reais que foram prometidas no .h
CordaViolao violao[6] = {
    {"E", "1a", 329.63},
    {"B", "2a", 246.94},
    {"G", "3a", 196.00},
    {"D", "4a", 146.83},
    {"A", "5a", 110.00},
    {"E", "6a", 82.41}
};

volatile int corda_atual = 0;

// Função interna de debounce
bool debounce_botao(uint32_t *ultimo_tempo) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - *ultimo_tempo > 200) {
        *ultimo_tempo = tempo_atual;
        return true;
    }
    return false;
}

// A função da interrupção (IRQ)
void botao_irq_handler(uint gpio, uint32_t events) {
    static uint32_t ultimo_tempo_A = 0;
    static uint32_t ultimo_tempo_B = 0;

    if (events & GPIO_IRQ_EDGE_FALL) {
        if (gpio == 5 && debounce_botao(&ultimo_tempo_A)) {
            corda_atual = (corda_atual - 1 + 6) % 6; // Volta
            printf("\n<<< VOLTOU\nCorda: %s | Alvo: %.2f Hz\n", violao[corda_atual].numero_corda, violao[corda_atual].freq_alvo);
        } 
        else if (gpio == 6 && debounce_botao(&ultimo_tempo_B)) {
            corda_atual = (corda_atual + 1) % 6; // Avança
            printf("\nAVANCOU >>>\nCorda: %s | Alvo: %.2f Hz\n", violao[corda_atual].numero_corda, violao[corda_atual].freq_alvo);
        }
    }
}

// A função pública que o main.c vai usar
void inicializar_botoes(void) {
    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5); // Botão A

    gpio_init(6);
    gpio_set_dir(6, GPIO_IN);
    gpio_pull_up(6); // Botão B

    gpio_set_irq_enabled_with_callback(5, GPIO_IRQ_EDGE_FALL, true, &botao_irq_handler);
    gpio_set_irq_enabled(6, GPIO_IRQ_EDGE_FALL, true);
}