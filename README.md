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
    <td><code>Kabely F-M a F-F</code></td>
    <td>Propojení všech součástek k nepájivému poli či k SMT8</td>
    <td></td>
  </tr>
  <tr>
    <td><code>Displej LCD</code></td>
    <td>Tady je spousta možností jaký použít. Já si osobně myslím, protože můj projekt je vyloženě o zobrazování času na displeji, tak pro vizualizaci odpočtu minut bude bohatě stačit řádkový displej SPI, proto jsem si zvolil řádkový displej max 7219 pro SPI</td>
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

**1) POPIS FUNKCE KÓDU**

* **Podrobnější vysvětlení jednotlivých částí projektu je přímo v kódu, jednoduše přejdětě do souboru [main.c](https://github.com/Patrik41089/MIT_MINUTKY/blob/main/src/main.c)**
* **Fyzicky je projekt řešen přes řádkový displej pro SPI, rotačním enkodérem a STM8S208RB připojným k PC**
* **Jak už může být zřejmé z názvu mého projektu, je hlavním úkolem odpočítávat minuty**

V primárním stavu, po nahrátí kódu (příkazem make flash) se zapne displej a rozsvítí se na něm hodnota času 180, která signalizuje maximální počet minut. Tento maximální počet minut jde upravit pomocí rotačního enkodéru. Po směru i proti směru otáčení se čas snižuje, pokud se chcete opět dostat na maximální hodnotu času (180 minut) tak se potřebujete otáčením enkodéru dostat na hodnotu 0, což automaticky přepne hodnotu na maximální čas (otáčení enkodérem je nastaveno přívětivě, aby nastavování probíhalo rychle a zárověn velice přesně při minimální rotaci).

Samotný **odpočet spustíte modrým tlačítkem USER**, které se nachází přímo na modulu SMT8S. Jakmile začne odpočet nebude již možné nastavovat čas pomocí otáčení enkodérem. Speciální funkcí také je, **když budete držet tlačítko USER**, **odpočet se pozastaví** po dobu dokud jej nepřestanete držet.

Jakmile odpočet skončí na displeji zůstane hodnota 000.00 a opět můžete enkodérem nastavit čas podle svých preferencí.


**2) SCHÉMA ZAPOJENÍ**

* Pro nahlédnutí přejdětě do [schéma.png](https://github.com/Patrik41089/MIT_MINUTKY/blob/main/Sch%C3%A9ma.png)

**3) BLOKOVÉ SCHÉMA**

* Pro nahlédnutí přejděte do [blokové_schéma.png](https://github.com/Patrik41089/MIT_MINUTKY/blob/main/BLOKOV%C3%89_SCH%C3%89MA.pdf)

  
