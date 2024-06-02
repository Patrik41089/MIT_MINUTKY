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

`Narozdíl od Pythonu je zde potřeba více souborů pro funkci našeho kodu v C pro STM8S. Knihovnu, tedy` [Toolchain](https://github.com/spseol/STM8S-toolchain) `máme z hodin MIT od pana učitele Nožky. V jiném případě je možné si knihovnu napsat sám podle potřeb nebo někde stáhnout.`

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
    <th colspan="3">Tabulka</th>
  </tr>
  <tr>
    <td><code>Nepájivé pole</code></td>
    <td>stáhne a nachystá knihovny</td>
    <td></td>
  </tr>
  <tr>
    <td><code>Enkodér</code></td>
    <td></td>
    <td><code>nebo tlačítka</code></td>
  </tr>
  <tr>
    <td colspan="3"><code>make flash</code> nahraje program do chipu. Na Linuxu se použije <a href="https://openocd.org/">OpenOCD</a>. Na Windows se použije <a href="https://www.st.com/en/development-tools/stvp-stm8.html">STVP</a> verze pro příkazový řádek.</td>
  </tr>
  <tr>
    <td colspan="3"><code>make flash2</code> záložní varianta, protože OpenOCD někdy nechce čip naprogramovat (používá <a href="https://github.com/vdudouyt/stm8flash">stm8flash</a>).</td>
  </tr>
  <tr>
    <td><code>make clean</code></td>
    <td>smaže všechno, co nakompiloval</td>
    <td></td>
  </tr>
  <tr>
    <td><code>make rebuild</code></td>
    <td>smaže vše a znovu zkompiluje</td>
    <td></td>
  </tr>
  <tr>
    <td><code>make openocd</code></td>
    <td>pustí <code>openocd</code> pro debug</td>
    <td></td>
  </tr>
  <tr>
    <td colspan="3"><code>make debug</code> spustí STM8-gdb</td>
  </tr>
</table>

  
