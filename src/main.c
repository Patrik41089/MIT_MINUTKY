#include "main.h"
#include "milis.h"
#include <stdbool.h>
#include <stdio.h>
#include <stm8s.h>
// #include "delay.h"
#include "max7219.h"
#include "stm8s_spi.h"
// #include "stm8s_tim1.h" // Přidán header pro TIM1
// #include "stm8s_tim4.h" // Přidán header pro TIM4

// Definice pinu pro akustický signál
#define BUZZER_PIN GPIO_PIN_3
#define BUZZER_PORT GPIOC

// Definice pinů pro enkoder
#define ENCODER_SW_PIN GPIO_PIN_5 // SW
#define ENCODER_SW_PORT GPIOA

#define ENCODER_DT_PIN GPIO_PIN_4 // DT
#define ENCODER_DT_PORT GPIOA

#define ENCODER_CLK_PIN GPIO_PIN_3 // CLK
#define ENCODER_CLK_PORT GPIOA

// Definice pinů pro řádkový displej SPI
#define DIN_PIN GPIO_PIN_7 // DIN
#define DIN_PORT GPIOB

#define CLK_PIN GPIO_PIN_4 // CLK
#define CLK_PORT GPIOD

#define CS_PIN GPIO_PIN_6 // CS
#define CS_PORT GPIOB

// Definice user tlačítka
#define BUTTON_PIN GPIO_PIN_4
#define BUTTON_PORT GPIOE

// Maximální čas v minutách
#define MAX_TIME 180

// Proměnná pro uložení zbývajícího času
volatile uint32_t n = MAX_TIME;
// Definice globální proměnné pro indikaci stisknutí tlačítka
volatile bool tlacitko_stisknuto = false;

// Prototypy funkcí
void Encoder_GPIO_Init(void);
void init_peripherals();
void beep();
void process_time_change(int8_t direction);
void init_spi();
int8_t Read_Encoder(void);
void init(void);
void display(uint8_t address, uint8_t data);
void update_display(int32_t value);
void update_display_from_encoder();
void init_timer();
void GPIO_Init_UserButton(void);
void EXTI_Init_UserButton(void);
void preruseni(void);

void Encoder_GPIO_Init(void) {
    // Povolit hodiny pro port GPIOA (není třeba, je implicitně povoleno u STM8)

    // Nastavení SW pinu jako vstup bez pull-up odporu
    GPIO_Init(ENCODER_SW_PORT, ENCODER_SW_PIN, GPIO_MODE_IN_FL_NO_IT);

    // Nastavení DT pinu jako vstup bez pull-up odporu
    GPIO_Init(ENCODER_DT_PORT, ENCODER_DT_PIN, GPIO_MODE_IN_FL_NO_IT);

    // Nastavení CLK pinu jako vstup bez pull-up odporu
    GPIO_Init(ENCODER_CLK_PORT, ENCODER_CLK_PIN, GPIO_MODE_IN_FL_NO_IT);
}

// Inicializace periferií
void init_peripherals() {
    // Inicializace GPIO pro akustický signál
    GPIO_Init(BUZZER_PORT, BUZZER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

    // Inicializace GPIO pro enkoder jako vstupy s pull-up odpory
    // GPIO_Init(ENCODER_PORT, ENCODER_PIN_1 | ENCODER_PIN_2,
    // GPIO_MODE_IN_PU_IT);

    // Nastavení interruptu pro enkoder (nepotřebné, pokud nepoužíváte
    //  interrupt)
    // EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
    // EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}

// Funkce pro zpracování akustického signálu
void beep() {
    GPIO_WriteReverse(BUZZER_PORT, BUZZER_PIN);
    // Počkejte krátkou dobu, abyste mohli slyšet zvuk
    for (int i = 0; i < 10000; i++)
        GPIO_WriteReverse(BUZZER_PORT, BUZZER_PIN);
}

// Funkce pro zpracování změny času
void process_time_change(int8_t direction) {
    if (direction == 1 && n < MAX_TIME) {
        n--;
    } else if (direction == -1 && n > 0) {
        n--;
    }
}

// Inicializace SPI
void init_spi() {
    // Povolit hodiny pro SPI
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
    // Inicializace SPI v master módu, rychlost 1 MHz, CPOL a CPHA (mód 0)
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER,
             SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE,
             SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
    // Povolit SPI
    SPI_Cmd(ENABLE);
    // Inicializace CS pinu jako výstupního
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void SPI_SendData_ToDisplay(uint8_t data) {
    GPIO_WriteLow(CS_PORT, CS_PIN); // CS Low to select the slave
    SPI_SendData(data);
    while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET)
        ;                            // Wait for transmission to complete
    GPIO_WriteHigh(CS_PORT, CS_PIN); // CS High to deselect the slave
}

int8_t Read_Encoder(void) {
    static uint8_t last_DT_state = 0;
    uint8_t current_CLK_state =
        GPIO_ReadInputPin(ENCODER_CLK_PORT, ENCODER_CLK_PIN);
    uint8_t current_DT_state =
        GPIO_ReadInputPin(ENCODER_DT_PORT, ENCODER_DT_PIN);

    int8_t encoder_value = 0;

    if (current_CLK_state != last_DT_state) {
        if (current_DT_state != current_CLK_state) {
            encoder_value = -1; // Clockwise
        } else {
            encoder_value = -1; // Counterclockwise
        }
    }

    last_DT_state = current_CLK_state;

    return encoder_value;
}

void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();
}

void display(uint8_t address, uint8_t data) {
    uint32_t mask;
    LOW(CS); // začátek přenosu

    /* pošlu adresu */
    mask = 128;
    mask = 1 << 7;
    mask = 0b10000000;
    while (mask) {
        if (address & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }
    /* pošlu data */
    mask = 0b10000000;
    while (mask) {
        if (data & mask) {
            HIGH(DIN);
        } else {
            LOW(DIN);
        }
        HIGH(CLK);
        mask = mask >> 1;
        LOW(CLK);
    }

    HIGH(CS); // konec přenosu
}

void update_display(int32_t value) {
    uint32_t digit0 = value % 10;
    uint32_t digit1 = (value / 10) % 10;
    uint32_t digit2 = (value / 100) % 10;

    display(DIGIT0, digit0);
    display(DIGIT1, digit1);
    display(DIGIT2, digit2);
}

/*void init_timer() {
    // Nastavení časovače TIM1
    TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 1000, 0); // Perioda 1ms

    // Povolení přerušení časovače TIM1
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
}
*/

void update_display_from_encoder() {
    int8_t encoder_change = Read_Encoder();
    if (encoder_change != 0) {
        process_time_change(encoder_change);
        if (n == 0) {
            n = MAX_TIME; // Nastavit čas na maximální hodnotu, pokud dosáhne
                          // nuly
        }
    }
}

void GPIO_Init_UserButton(void) {
    // Nastavení pinu PE4 jako vstup s pull-up rezistorem
    GPIO_Init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_IN_PU_IT);
}

// Inicializace přerušení pro tlačítko USER
void EXTI_Init_UserButton(void) {
    // Povolení přerušení na pádu hrany (falling edge) pro port D
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOE, EXTI_SENSITIVITY_FALL_ONLY);
}

// Obsluha přerušení pro port D
void preruseni(void) {
    if (GPIO_ReadInputPin(BUTTON_PORT, BUTTON_PIN) == RESET && n > 0) {
        tlacitko_stisknuto = true;
    }
}

/* ENKODER TLACITKO
static bool encoder_button_pressed = false;


void EXTI_Configuration(void) {
    // Konfigurace tlačítka enkodéru jako externího přerušení
    EXTI_DeInit(); // Reset konfigurace EXTI
    EXTI_SetExtIntSensitivity(
        EXTI_PORT_GPIOA,
        EXTI_SENSITIVITY_FALL_ONLY); // Nastavení citlivosti na spád
}

// Funkce, která se vyvolá při stisknutí tlačítka enkodéru
void EXTI_SW_ISR(void) {
    encoder_button_pressed = true; // Nastavit příznak stisknutí tlačítka
}
*/
void main(void) {

    uint32_t time = 0;
    uint32_t time2 = 0;

    init();
    CLK_HSIPrescalerConfig(
        CLK_PRESCALER_HSIDIV1); // Nastavení hodinového prescaleru
    init_peripherals();
    init_spi();
    Encoder_GPIO_Init(); // Inicializace pinů enkodéru
    // init_timer();
    // EXTI_Configuration();
    // Inicializace GPIO
    GPIO_Init_UserButton();

    // Inicializace přerušení
    EXTI_Init_UserButton();

    // Povolení globálních přerušení
    enableInterrupts();

    display(DECODE_MODE, 0b11111111);
    display(SCAN_LIMIT, 7);
    display(INTENSITY, 4);
    display(DISPLAY_TEST, DISPLAY_TEST_OFF);
    display(SHUTDOWN, SHUTDOWN_ON);
    display(DIGIT0, 0xF);
    display(DIGIT1, 0xF);
    display(DIGIT2, 0xF);
    display(DIGIT3, 0xF);
    display(DIGIT4, 0xF);
    display(DIGIT5, 0xF);
    display(DIGIT6, 0xF);
    display(DIGIT7, 0xF);

    while (1) {

        if (milis() - time > 1) {
            time = milis();
            update_display_from_encoder();
            update_display(n);
            // Aktualizace displeje s novou hodnotou zbývajícího času
            char display_buffer[4];
            sprintf(display_buffer, "%03d", n);
            for (uint8_t i = 0; i < 3; i++) {
                SPI_SendData_ToDisplay(display_buffer[i]);
            }
        }
        if (milis() - time2 > 1000) {
            time2 = milis();
            if (tlacitko_stisknuto && n > 0) {
                n--;
            }
            if (n == 0) {
                tlacitko_stisknuto = false;
            }
        }
    }
}