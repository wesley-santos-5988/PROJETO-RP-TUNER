#ifndef LED_PWM_H
#define LED_PWM_H

void pwm_init_led(void);
void pwm_atualizar_brilho(int brilho); // <-- Agora sim, ele sabe que só precisa de 1 argumento (o brilho)

#endif // LED_PWM_H