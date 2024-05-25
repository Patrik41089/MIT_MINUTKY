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
void process_time_change(int8_t direction) {
    if (direction == 1 && remaining_time < MAX_TIME) {
        remaining_time++;
    } else if (direction == -1 && remaining_time > 0) {
        remaining_time--;
    }
}

// Deklarace funkcí pro LCD
void LCD_Init();
void LCD_Clear();
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);

// Funkce pro inicializaci LCD
void LCD_Init() {
    // Implementace inicializace LCD
    // Např. Reset LCD, nastavení SPI, poslání inicializačních příkazů
}

// Funkce pro vyčištění LCD
void LCD_Clear() {
    LCD_SendCommand(0x01); // Předpokládejme, že 0x01 je příkaz pro vyčištění LCD
}

// Funkce pro odeslání příkazu na LCD
void LCD_SendCommand(uint8_t cmd) {
    // Implementace odeslání příkazu přes SPI
    // Např. nastavení DC pinu na LOW, poslání dat přes SPI
    (void)cmd; // Označení argumentu jako použitý
}

// Funkce pro odeslání dat na LCD
void LCD_SendData(uint8_t data) {
    // Implementace odeslání dat přes SPI
    // Např. nastavení DC pinu na HIGH, poslání dat přes SPI
    (void)data; // Označení argumentu jako použitý
}

// Inicializace SPI
void init_spi() {
    // Reset SPI periférie
    SPI_DeInit();

    // Inicializace SPI v master módu, rychlost 1 MHz, CPOL a CPHA (mód 0)
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, 
             SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, 
             SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);

    // Povolit SPI
    SPI_Cmd(ENABLE);
}

int main() {
    // Inicializace periferií
    init_peripherals();
    init_timer();
    init_spi();
    LCD_Init();
    // Vyčištění displeje
    LCD_Clear();

    // Nekonečná smyčka
    while (1) {
        // Vaše hlavní smyčka kódu
        // Přidání vaší logiky zde
    }
    // Tento kód nebude nikdy dosažen
    // return 0; // Odstranění nebo přemístění returnu mimo nekonečnou smyčku

    return 0;
}