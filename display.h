#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "pico/stdlib.h"

// Definições do ecrã OLED SSD1306
#define OLED_ADDR   0x3C
#define OLED_WIDTH  128
#define OLED_HEIGHT 64

// Funções base do OLED
void oled_init(void);
void oled_clear(void);
void oled_update(void);
void oled_draw_char(uint8_t x, uint8_t y, char c);
void oled_draw_string(uint8_t x, uint8_t y, const char *str);

// Função específica do nosso projeto
void atualizar_ecra_afinacao(float freq_lida);

#endif // DISPLAY_H