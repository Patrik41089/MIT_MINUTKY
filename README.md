PROJEKT MINUTKY
=
`Toto je krok za krokem jak je potřeba postupovat a jak jsem postupoval já`

*Upozornění*
=

* *Pokud budete chtít pracovat na projektu přes VirtualBox je důležité si stáhnout aktuální [Toolchain](https://github.com/spseol/STM8S-toolchain), protože vám jinak nebude fungovat komunikace pro Github!*
* *Dále nesmíte zapomenout v nastavení Virtualboxu zapnout povolení portu USB pro STM8! Jednoduše při zapnutí Virtualboxu jdětě do nastavení => USB => povolit USB ovladač => a vybrat správný USB port (STMicroelectronics STM32 STLink)*

**1) Prostředí pro možnost kompilace kódu v C**

`Jako první je potřeba si zajistit prostředí ve kterém se bude programovat pro jazyk C. Vzhledem k tomu, že na Windows to jen tak s Visual studio nejde tak jsou dvě možnosti:`
* Nainstalovat kompilátor pro Windows (například Cosmic)
  * nebo
* Nainstalovat Linux (já jsem si nainstaloval [Oracle OM Virtualbox](https://www.virtualbox.org/), který má v sobě oprační systém Linux)

**2) Knihovna**

`Narozdíl od Pythonu je zde potřeba více souborů pro funkci našeho kódu v C pro STM8S. Knihovnu, tedy` [Toolchain](https://github.com/spseol/STM8S-toolchain) `máme z hodin MIT od pana učitele Nožky. V jiném případě je možné si knihovnu napsat sám podle potřeb nebo někde stáhnout.`

**3) Rozběh knihovny**

`Kompilaci a spuštění naší knihovny popsal velice dobře pan učitel Nožka v readme, které jsem přiložil v tomto projektu do složky` [archived_files](https://github.com/Patrik41089/MIT_MINUTKY/tree/main/archived_files) `Zkráceně lze říci pokud to po kompilaci pomocí make nevyhodí žádný error a bude tam vidět velikost stažení úplně dole tak vše pravděpodobně proběhlo v pořádku`

**4) Kód**

`Teď už zbývá napsat kód pro můj projekt. Převážně se kód píše pouze do souboru main.c, protože knihovnu už máme, ale pokud bych chtěl použít například nějaké přerušení tak bych ho musel dopsat ještě do souboru stm8s_it.c. Taky nezapomenout, že pokud budeme některý z modulů používat je nutné si hlavičkový soubor (koncovka .h) z lib (library) převést do inc (included).`

**5) Kompilace**

`Je dobré vědět, že klasickou kompilaci kódu lze provést i bez připojeného STM8 k PC (pomocí make), ale následné otestování a nahrátí (make flash) do STM8 lze samozřejmě pouze s STM8 připojeným k PC (výše jsem psal jak zajistit, aby nenastala chyba při nezapnutém povolení pro USB porty). Pokud kompilace nevyhazuje errory tak kód je zkompilovaný a lze jej nahrát do STM8.`

**6) Součástky**

`Asi nejdůležitější část si pořádně zkontrolovat jestli máme vše pro fyzické sestavení našeho projektu (což je bohužel část na které jsem se zasekl nejvíc). Pro svůj projekt budu potřebovat:`

<table>
  <tr>
    <th colspan="3">Tabulka potřebných součástek</th>
  </tr>
  <tr>
    <td><code>STM8</code></td>
    <td>Propojení součástek a připojení pro napájení a komunikaci s PC</td>
    <td></td>
  </tr>
  <tr>
    <td><code>Nepájivé pole</code></td>
    <td>Propojení potřebných součástek (některé součástky nejspíš budou stačit propojit přímo k STM8)</td>
    <td></td>
  </tr>
  <tr>
    <td><code>Enkodér</code></td>
    <td>Nastavení minut pomocí otáčení enkodéru (podle mého vybraného zadání 1-180 minut), já jsem si raději vybral enkodér než možnost tlačítek</td>
    <td><code>nebo tlačítka</code></td>
  </tr>
  <tr>
    <td><code>Reproduktor</code></td>
    <td>Po skončení odpočtu minut se ozve akustický signál, já jsem si zvolil jednoduchý Buzzer, protože bude bohatě stačit</td>
    <td><code>nebo Buzzer</code></td>
  </tr>
  <tr>
    <td><code>Kabely F-M</code></td>
    <td>Propojení všech součástek k nepájivému poli či k SMT8</td>
    <td></td>
  </tr>
  <tr>
    <td><code>Displej LCD</code></td>
    <td>Tady je spousta možností jaký použít. Já si osobně myslím, protože můj projekt je vyloženě o zobrazování na displeji, pro vizualizaci minut bude nejlepší vzít displej na kterém čísla budou hezky vidět, proto jsem si zvolil LCD</td>
    <td><code>nebo třeba řádkový SPI</code></td>
  </tr>
</table>

**7) Testování**

`Je asi zřejmé, že na první pokus to nebude fungovat jak si představujeme a budeme postupně opakovat stejné kroky:`
* Postupná úprava kódu, aby nám to fungovalo jak požadujeme
* Kompilace a nahrátí kódu do SMT8
* Následné testování na součástkách

Závěr
=

**1) VÝVOJOVÝ DIAGRAM**

**2) POPIS FUNKCE KÓDU**
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm8s.h"
#include "stm8s_spi.h"
#include "uart1.h"
#include "max7219.h"


// Definice pinu pro akustický signál
#define BUZZER_PIN GPIO_PIN_3
#define BUZZER_PORT GPIOC

// Definice pinů pro enkoder
#define ENCODER_SW_PIN GPIO_PIN_1   // SW
#define ENCODER_SW_PORT GPIOA     

#define ENCODER_DT_PIN GPIO_PIN_2   // DT
#define ENCODER_DT_PORT GPIOA 

#define ENCODER_CLK_PIN GPIO_PIN_3  // CLK
#define ENCODER_CLK_PORT GPIOA    

// Definice pinů pro řádkový displej SPI
#define SPI_MOSI_PIN GPIO_PIN_7  // DIN
#define SPI_MOSI_PORT GPIOB       

#define SPI_SCK_PIN GPIO_PIN_4   // CLK
#define SPI_SCK_PORT GPIOD      

#define SPI_CS_PIN GPIO_PIN_6  // CS
#define SPI_CS_PORT GPIOB      

// Maximální čas v minutách
#define MAX_TIME 180

// Proměnná pro uložení zbývajícího času
volatile uint8_t remaining_time = 100;

// Prototypy funkcí
void Encoder_GPIO_Init(void);
void SPI_Init_Display(void);
void SPI_SendData_ToDisplay(uint8_t data);
void delay_ms(uint16_t ms);
int8_t Read_Encoder(void);

void Encoder_GPIO_Init(void)
{
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
    //GPIO_Init(ENCODER_PORT, ENCODER_PIN_1 | ENCODER_PIN_2, GPIO_MODE_IN_PU_IT);

    // Nastavení interruptu pro enkoder (nepotřebné, pokud nepoužíváte interrupt)
    //EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
    //EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}

// Funkce pro zpracování akustického signálu
void beep() {
    GPIO_WriteReverse(BUZZER_PORT, BUZZER_PIN);
    // Počkejte krátkou dobu, abyste mohli slyšet zvuk
    for(int i = 0; i < 10000; i++)
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
    GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void SPI_Init_Display(void)
{
    // Povolit hodiny pro porty
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

    // Inicializace SPI
    //SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, 
             //SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_1LINE_TX, 
             //SPI_NSS_SOFT, 0x07);
    SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER, 
             SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, 
             SPI_NSS_SOFT, 0x07);
    SPI_Cmd(ENABLE);

    // Inicializace CS pinu jako výstupního
    GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void SPI_SendData_ToDisplay(uint8_t data)
{
    GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN);  // CS Low to select the slave
    SPI_SendData(data);
    while (SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);  // Wait for transmission to complete
    GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN);  // CS High to deselect the slave
}

int8_t Read_Encoder(void)
{
    static uint8_t last_DT_state = 0;
    uint8_t current_CLK_state = GPIO_ReadInputPin(ENCODER_CLK_PORT, ENCODER_CLK_PIN);
    uint8_t current_DT_state = GPIO_ReadInputPin(ENCODER_DT_PORT, ENCODER_DT_PIN);

    int8_t encoder_value = 0;

    if (current_CLK_state != last_DT_state)
    {
        if (current_DT_state != current_CLK_state)
        {
            encoder_value = 1;  // Clockwise
        }
        else
        {
            encoder_value = -1;  // Counterclockwise
        }
    }

    last_DT_state = current_CLK_state;

    return encoder_value;
}

void delay_ms(uint16_t ms)
{
    for (uint16_t i = 0; i < ms; i++)
    {
        for (uint16_t j = 0; j < 1600; j++)
        {
            __asm__("nop");
        }
    }
}

void main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  // Nastavení hodinového prescaleru
    init_peripherals();
    init_spi();
    Encoder_GPIO_Init();  // Inicializace pinů enkodéru
    SPI_Init_Display();   // Inicializace SPI pro displej

    while (1)
    {
        int8_t encoder_change = Read_Encoder();
        if (encoder_change != 0)
        {
            remaining_time += encoder_change;
            if (remaining_time > MAX_TIME)
            {
                remaining_time = MAX_TIME;
            }
            else if (remaining_time < 0)
            {
                remaining_time = 0;
            }
        }

        // Aktualizace displeje s novou hodnotou zbývajícího času
        char display_buffer[4];
        sprintf(display_buffer, "%03d", remaining_time);
        for (uint8_t i = 0; i < 3; i++)
        {
            SPI_SendData_ToDisplay(display_buffer[i]);
        }

        // Odpocet, pokud je zbývající čas větší než nula
    if (remaining_time > 0)
    {
        delay_ms(1000); // Počkej jednu sekundu
        remaining_time--; // Sniz čas o jednu sekundu
    }
        else
        {
            // Akustický signál po dosažení nuly
            for (int i = 0; i < 5; i++)
            {
                beep(); // Aktivace akustického signálu
                delay_ms(1000); // Počkej jednu sekundu
            }
        }
    }
}

NOZKAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "max7219.h"

/*#define DIN_PORT GPIOB
#define DIN_PIN GPIO_PIN_4
#define CS_PORT GPIOB
#define CS_PIN GPIO_PIN_3
#define CLK_PORT GPIOB
#define CLK_PIN GPIO_PIN_2
*/

#define DIN_PIN GPIO_PIN_7  // DIN
#define DIN_PORT GPIOB       

#define CLK_PIN GPIO_PIN_4   // CLK
#define CLK_PORT GPIOD      

#define CS_PIN GPIO_PIN_6  // CS
#define CS_PORT GPIOB   
void init(void) {
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); // taktovani MCU na 16MHz

    GPIO_Init(DIN_PORT, DIN_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(CS_PORT, CS_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(CLK_PORT, CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    init_milis();

}

void display(uint8_t address, uint8_t data) {
    uint8_t mask;
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

int main(void) {

    uint32_t time = 0;
    uint8_t number = 1;

    init();

    display(DECODE_MODE, 0b11111111);
    display(SCAN_LIMIT, 7);
    display(INTENSITY, 1);
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
        if (milis() - time > 333) {
            time = milis();
            display(DIGIT0, number);
            display(DIGIT1, number);
            display(DIGIT2, number);
            display(DIGIT3, number);
            display(DIGIT4, number);
            display(DIGIT5, number);
            display(DIGIT6, number);
            display(DIGIT7, number);
            number++;
        }
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"


VYPOCETTTTTTTTTTTTTTTTTTTTTTTT
        uint32_t d1 = n%10;
        uint32_t d2 = n/10;
        n = n%100;
        uint32_t d3 = n/100;
        n = n%1000;
        uint32_t d4 = n/1000;
        n = n%10000;
        uint32_t d5= n/10000;
        n= n%1000000;
        uint32_t d6 = n/1000000;
        n= n%10000000;
        uint32_t d7 = n/10000000;
        n = n%10000000;
        uint32_t d8 = n/100000000;
`


  
