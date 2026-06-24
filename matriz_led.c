#include "matriz_led.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Código de máquina compilado para controlar os LEDs WS2812 (Neopixels)
// Isso evita que você precise criar arquivos .pio externos!
static const uint16_t ws2812_program_instructions[] = {
    0x6221, 0x1123, 0x1400, 0xa442
};
static const struct pio_program ws2812_program = {
    .instructions = ws2812_program_instructions,
    .length = 4,
    .origin = -1,
};

static uint32_t leds[NUM_LEDS];
static PIO pio = pio0;
static uint sm = 0;

// Converte RGB para o formato de 32 bits que o LED entende
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Inicializa a comunicação PIO com a matriz
void matriz_init(void) {
    uint offset = pio_add_program(pio, &ws2812_program);
    
    pio_gpio_init(pio, LED_PIN);
    pio_sm_set_consecutive_pindirs(pio, sm, LED_PIN, 1, true);
    
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + 0, offset + 3);
    sm_config_set_sideset(&c, 1, false, false);
    sm_config_set_sideset_pins(&c, LED_PIN);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    
    float div = clock_get_hz(clk_sys) / (800000.0 * 10);
    sm_config_set_clkdiv(&c, div);
    
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
    
    matriz_clear();
    matriz_update();
}

void matriz_clear(void) {
    for(int i=0; i<NUM_LEDS; i++) leds[i] = 0;
}

void matriz_update(void) {
    for(int i=0; i<NUM_LEDS; i++) {
        pio_sm_put_blocking(pio, sm, leds[i] << 8u);
    }
}

// Função auxiliar para pintar um LED usando coordenadas X e Y
void set_led(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || x > 4 || y < 0 || y > 4) return;
    int index = y * 5 + x; // Transforma 2D em 1D
    leds[index] = urgb_u32(r, g, b);
}

// A lógica visual do nosso afinador
void atualizar_agulha_led(float freq_lida, float freq_alvo) {
    matriz_clear();
    
    // Se não estiver tocando nada, apaga tudo
    if (freq_lida == 0.0f) {
        matriz_update();
        return;
    }

    float erro = freq_lida - freq_alvo;
    int coluna = 2; // Padrão: Centro (Afinado)

    // Define qual coluna acender baseado na distância do alvo
    if (erro < -5.0) coluna = 0;       // Muito grave
    else if (erro < -1.0) coluna = 1;  // Quase (grave)
    else if (erro > 5.0) coluna = 4;   // Muito agudo
    else if (erro > 1.0) coluna = 3;   // Quase (agudo)

    // Acende a coluna inteira
    for (int y = 0; y < 5; y++) {
        if (coluna == 2) set_led(coluna, y, 0, 50, 0); // Verde (Afinado)
        else if (coluna == 1 || coluna == 3) set_led(coluna, y, 50, 50, 0); // Amarelo
        else set_led(coluna, y, 50, 0, 0); // Vermelho
    }
    
    matriz_update();
}