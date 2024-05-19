#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm8s.h"

// Definice pinu pro akustický signál
#define BUZZER_PIN GPIO_PIN_3
#define BUZZER_PORT GPIOC

// Definice pinů pro enkoder
#define ENCODER_PIN_A GPIO_PIN_1
#define ENCODER_PIN_B GPIO_PIN_2
#define ENCODER_PORT GPIOA

// Maximální čas v minutách
#define MAX_TIME 180

// Proměnná pro uložení zbývajícího času
uint8_t remaining_time = 0;

// Inicializace periferií
void init_peripherals() {
    // Inicializace GPIO pro akustický signál
    GPIO_Init(BUZZER_PORT, BUZZER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

    // Inicializace GPIO pro enkoder jako vstupy s pull-up odpory
    GPIO_Init(ENCODER_PORT, ENCODER_PIN_A | ENCODER_PIN_B, GPIO_MODE_IN_PU_IT);

    // Nastavení interruptu pro enkoder
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}

// Funkce pro inicializaci časovače
void init_timer() {
    // Nastavení časovače pro přerušení každou minutu
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
    TIM4_Cmd(ENABLE);
}

// Funkce pro zpracování akustického signálu
void beep() {
    GPIO_WriteReverse(BUZZER_PORT, BUZZER_PIN);
    // Počkejte krátkou dobu, abyste mohli slyšet zvuk
    for(int i = 0; i < 10000; i++);
    GPIO_WriteReverse(BUZZER_PORT, BUZZER_PIN);
}

// Funkce pro zpracování změny času
void process_time_change(uint8_t direction) {
    if (direction == 1 && remaining_time < MAX_TIME) {
        remaining_time++;
    } else if (direction == -1 && remaining_time > 0) {
        remaining_time--;
    }
}

// Hlavní funkce
int main() {
    // Inicializace periferií
    init_peripherals();
    init_timer();
    // Nekonečná smyčka
    while (1) {}
    return 0;
}