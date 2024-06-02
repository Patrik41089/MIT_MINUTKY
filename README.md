PROJEKT MINUTKY
=
`Toto je krok za krokem jak je potřeba postupovat a jak jsem postupoval já`

*Upozornění*
=

* *Pokud budete chtít pracovat na projektu přes VirtualBox je důležité si stáhnout aktuální [Toolchain](https://github.com/spseol/STM8S-toolchain), protože vám jinak nebude fungovat komunikace pro Github!*
* *Dále nesmíte zapomenout v nastavení Virtualboxu zapnout povolení portu USB pro STM8! Jednoduše při zapnutí Virtualboxu jdětě do nastavení => USB => povolit USB ovladač => a vybrat správný USB port (STMicroelectronics STM32 STLink)*

**1) Prostředí pro možnost kompilace kódu v C**

`Jako první je potřeba si zajistit prostředí ve kterém se bude programovat pro jazyk C. Vzhledem k tomu, že na Windows to jen tak s visual studio nejde tak jsou dvě možnosti:`
* Nainstalovat kompilátor pro Windows (například Cosmic)
  * nebo
* Nainstalovat Linux (já jsem si nainstaloval [Oracle OM Virtualbox](https://www.virtualbox.org/), který má v sobě oprační systém Linux)

**2) Knihovna**

`Narozdíl od Pythonu je zde potřeba více souborů pro funkci našeho kodu v C pro STM8S. Knihovnu, tedy` [Toolchain](https://github.com/spseol/STM8S-toolchain) `máme z hodin MIT od pana učitele Nožky. V jiném případě je možné si knihovnu napsat sám podle potřeb nebo někde stáhnout.`

**3) Rozběh knihovny**

`Kompilaci a spuštění naší knihovny popsal velice dobře pan učitel Nožka v readme, které jsem přiložil v tomto projektu do složky` [archived_files](https://github.com/Patrik41089/MIT_MINUTKY/tree/main/archived_files) `Zkráceně lze říci pokud to po kompilaci pomocí make nevyhodí žádný error a bude tam vidět velikost stažení úplně dole tak vše pravděpodobně proběhlo v pořádku`

**4) Kód**

`Teď už zbývá napsat kód pro múj projekt. Převážně se kód píše pouze do souboru main.c, protože knihovnu už máme, ale pokud bych chtěl použít například nějaké přerušení tak bych ho musel dopsat ještě do souboru stm8s_it.c`

**5) Kompilace**
`Je dobré vědět, že klasickou kompilaci kódu lze provést pouze při připojeném STM8 k PC` 
  
