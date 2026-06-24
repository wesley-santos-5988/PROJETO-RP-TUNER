#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// =========================================================
// 1. DEFINIÇÕES DO I2C (Adaptado para a BitDogLab v6.3)
// =========================================================
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// =========================================================
// 2. ESTRUTURA DE DADOS DAS CORDAS DO VIOLÃO
// =========================================================
typedef struct {
    char nome_nota[3];    // Ex: "E", "A"
    char numero_corda[4]; // Ex: "1a", "2a"
    float freq_alvo;      // Frequência ideal em Hz
} CordaViolao;

CordaViolao violao[6] = {
    {"E", "1a", 329.63}, // Mi agudo
    {"B", "2a", 246.94}, // Si
    {"G", "3a", 196.00}, // Sol
    {"D", "4a", 146.83}, // Ré
    {"A", "5a", 110.00}, // Lá
    {"E", "6a", 82.41}   // Mi grave
};

volatile int corda_atual = 0; // Começa na 1ª corda

// =========================================================
// 3. VARIÁVEIS DO ADC (Microfone) E TIMER
// =========================================================
#define TAMANHO_BUFFER 1024
uint16_t buffer_amostras[TAMANHO_BUFFER];
volatile int indice_amostra = 0;
volatile bool buffer_cheio = false;

// Função chamada pelo Timer a cada 125 microssegundos (8 kHz)
bool timer_amostragem_callback(struct repeating_timer *t) {
    if (!buffer_cheio) {
        buffer_amostras[indice_amostra] = adc_read(); // Lê o pino GP28
        indice_amostra++;
        if (indice_amostra >= TAMANHO_BUFFER) {
            buffer_cheio = true; 
            indice_amostra = 0;  
        }
    }
    return true; // Mantém o timer em loop
}

// =========================================================
// 4. INTERRUPÇÃO DOS BOTÕES (IRQ) COM DEBOUNCE
// =========================================================
bool debounce_botao(uint32_t *ultimo_tempo) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - *ultimo_tempo > 200) { // 200ms de rejeição de ruído mecânico
        *ultimo_tempo = tempo_atual;
        return true;
    }
    return false;
}

void botao_irq_handler(uint gpio, uint32_t events) {
    static uint32_t ultimo_tempo_A = 0;
    static uint32_t ultimo_tempo_B = 0;

    // Apenas processa se o evento for de fato o aperto do botão (borda de descida)
    if (events & GPIO_IRQ_EDGE_FALL) {
        
        if (gpio == 5 && debounce_botao(&ultimo_tempo_A)) {
            // Botão A (GP5): Volta uma corda
            corda_atual = (corda_atual - 1 + 6) % 6; 
            
            printf("\n<<< VOLTOU\n");
            printf("Corda Selecionada: %s\n", violao[corda_atual].numero_corda);
            printf("Nota Alvo: %s (%.2f Hz)\n", violao[corda_atual].nome_nota, violao[corda_atual].freq_alvo);
        } 
        else if (gpio == 6 && debounce_botao(&ultimo_tempo_B)) {
            // Botão B (GP6): Avança uma corda
            corda_atual = (corda_atual + 1) % 6; 
            
            printf("\nAVANCOU >>>\n");
            printf("Corda Selecionada: %s\n", violao[corda_atual].numero_corda);
            printf("Nota Alvo: %s (%.2f Hz)\n", violao[corda_atual].nome_nota, violao[corda_atual].freq_alvo);
        }
    }
}

// =========================================================
// FUNÇÃO PRINCIPAL (MAIN)
// =========================================================
int main() {
    stdio_init_all(); // Inicializa o console USB para os printf()

    // --- Inicialização do I2C (Display) ---
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // --- Inicialização do ADC (Microfone/Potenciômetro) ---
    adc_init();
    adc_gpio_init(28);   // GP28 habilitado para leitura analógica
    adc_select_input(2); // Canal analógico 2 corresponde ao GP28

    // --- Inicialização dos Botões ---
    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5); // Botão A

    gpio_init(6);
    gpio_set_dir(6, GPIO_IN);
    gpio_pull_up(6); // Botão B

    // Associa os botões à função de interrupção (Borda de descida)
    gpio_set_irq_enabled_with_callback(5, GPIO_IRQ_EDGE_FALL, true, &botao_irq_handler);
    gpio_set_irq_enabled(6, GPIO_IRQ_EDGE_FALL, true);

    // --- Inicialização do Timer de Amostragem (8000 Hz) ---
    struct repeating_timer timer;
    add_repeating_timer_us(-125, timer_amostragem_callback, NULL, &timer);

    printf("RP TUNER INICIADO! Aperte os botoes para trocar de corda.\n");

    // Loop Principal
    while (true) {
        // Se o timer encher o buffer, processamos o som (aqui entrará a matemática depois)
        if (buffer_cheio) {
            // Futuro: Processar algoritmo de Cruzamento por Zero / FFT
            // Para não travar tudo agora, apenas esvaziamos o buffer silenciosamente:
            buffer_cheio = false; 
        }
        
        sleep_ms(10); // Pausa leve para não gastar energia 100% do tempo
    }
}