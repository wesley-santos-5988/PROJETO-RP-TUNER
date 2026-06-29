#ifndef LED_PWM_H
#define LED_PWM_H

void pwm_init_led(void);
void pwm_atualizar_brilho(float freq_lida, float freq_alvo);

#endif // LED_PWM_H