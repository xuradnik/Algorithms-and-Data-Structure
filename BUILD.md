# Testy
Projekt obsahuje jednoduché jednotkové testy implementované v priečinku `libds-tests`. Testy sú zahrnuté do build systému, takže sa pri použití príkazu `ninja` v priečinku `build` automaticky prekladajú. Selektívny preklad testov je možný príkazom:
```bash
ninja ds-tests
```
Všetky testy spustíme príkazom:
```bash
./libds.tests/ds-tests
```
Spustiteľný súbor s testami má niekoľko prepínačov, ktoré je možné zobraziť prepínačom `--help`:
```bash
./libds.tests/ds-tests --help
```
Podrobný výstup testov zobrazíme prepínačom `--detail`:
```bash
./libds.tests/ds-tests --detail
```
Zoznam dostupných testov zobrazíme prepínačom `--list`:
```bash
./libds.tests/ds-tests --list
```
Konkrétne testy je možné spustiť prepínačom `--test`, pričom použijeme plne kvalifikovaný názov testu získaný z výstupu prepínača `--list`. Jednotlivé testy oddleujeme čiarkou:
```bash
./libds.tests/ds-tests.exe --test Tests.SinglyLinkedSequence,Tests.DoublyLinkedSequence
```




# Dokumentácia

## UML
Projekt obsahuje pre jednotlivé cvičenia dokumentáciu vo forme UML diagramov. Na vytvorenie diagramov je použitý nástroj [PlantUML](https://plantuml.com/). Tento nástroj vytvára UML diagramy na základe textového popisu. Tieto popisy sa nachádzajú v priečinku:
```
doc/uml/
```
Na vygenerovanie diagramov môžeme použiť rozšírenie [PlantUML](https://marketplace.visualstudio.com/items?itemName=jebbs.plantuml). Nainštalujeme ho príkazom:
```bash
code --install-extension jebbs.plantuml --force
```
UML diagram vygenerujeme otvorením zdrojového súboru (`*.puml`) a stlačením klávesovej skratky `Alt+D`. UML diagramy je možné rendrovať **lokálne** alebo na **serveri**.

### Rendrovanie na serveri
Do používateľských nastavení je potrebné pridať adresu servera. Tu je možné použiť voľe dostupný oficiálny server. Používateľské nastavenia otvoríme skratkou `Ctrl+Shift+P`, napísaním a výberom možnosti **Open User Settings (JSON)**. Do otvoreného JSON súboru je potrebné pridať tieto nastavenia:
```JSON
{
    "plantuml.render": "PlantUMLServer",
    "plantuml.server": "https://www.plantuml.com/plantuml",
}
```

### Lokálne rendrovanie
Pre lokálne rendrovanie je potrebné, aby v prostredí, z ktorého spúšťame Visual Studio Code, bola nainštalovaná a viditeľná (v premennej `PATH`) Java.

Najprv je potrebné zistiť, či je už v systéme Windows nainštalovaná. Otvoríme **PowerShell** a zadáme príkaz:
```PowerShell
Get-Command java
```
Ak je nainštalovaná, zobrazí sa nám jej umiestnenie, v opačnom prípade uvidíme chybu a Javu bude potrebné nainštalovať, napríklad príkazom:
```PowerShell
winget install --id BellSoft.LibericaJDK.25 --exact --source winget
```

Ak chceme aby bola Java viditeľná aj pri spustení Visual Studia Code z interpretera Bash príkazom:
```bash
code .
```
musíme upraviť premennú `PATH` v súbore `~/.bashrc`:
```bash
echo "export PATH=\"/c/Program Files/BellSoft/LibericaJDK-25/bin:\$PATH\"" >> ~/.bashrc
```
Cestu `/c/Program Files/BellSoft/LibericaJDK-25/bin` je, pochopiteľne, potrebné upraviť podľa umiestnenia Javy získanom vyššie popísaným spôsobom. Nezabudnite na použitie dopredných `/`.



## Doxygen
Všetky triedy a funkcie obsahujú dokumentačné komentáre. Na základe dokumentačných komentárov je možné vygenerovať externú dokumentáciu vo formáte HTML pomocou nástroja [Doxygen](https://www.doxygen.nl/index.html). V prostredí UCRT64 ho nainštalujeme príkazom:
```bash
pacman -S mingw-w64-ucrt-x86_64-doxygen
```
Dokumentáciu vygenerujeme príkazom spusteným v koreni projektu:
```bash
doxygen doc/Doxyfile
```
Vygenerovaná dokumentácia sa uloží do priečinku:
```
doc/html/
```
Dokumentáciu otvoríme otvorením súboru:
```
doc/html/index.html
```
vo webovom prehliadači.



## Semestrálna práca
Dokumentáciu semestrálnej práce vypracujte v šablóne, ktorú nájdete v súbore `doc/semesteral_project/documentation.markdown`. Šablóna používa formát [Pandoc markdown](https://garrettgman.github.io/rmarkdown/authoring_pandoc_markdown.html). Základnú podobu výsledného dokumentu vám ukáže Visual Studio Code.

### Vygenerovanie PDF verzie dokumentácie
**Vytvorenie PDF verzie nemusíte robiť, budú to robiť vyučujúci. Ak chcete, môžete zreplikovať náš postup.**

 Na vytvorenie PDF súboru použijeme nástroj [Pandoc](https://pandoc.org/). V repozitároch projektu MSYS sa tento nástroj, nanešťastie, nenachádza, preto ho je potrebné nainštalovať v systéme Windows nasledujúcim príkazom spusteným v interpreteri PowerShell:
```PowerShell
winget install --id JohnMacFarlane.Pandoc --exact --source winget
```
Pandoc na vytvorenie PDF súboru používa [LaTeX](https://www.latex-project.org/). Tento sa už nachádza v repozitároch projektu MSYS, preto ho nainštalujeme nasledujúcim príkazom spusteným v interpreteri Bash (UCRT64):
```Bash
pacman -S \
  mingw-w64-ucrt-x86_64-texlive-bibtex-extra \
  mingw-w64-ucrt-x86_64-texlive-bin \
  mingw-w64-ucrt-x86_64-texlive-core \
  mingw-w64-ucrt-x86_64-texlive-extra-utils \
  mingw-w64-ucrt-x86_64-texlive-font-utils \
  mingw-w64-ucrt-x86_64-texlive-fonts-extra \
  mingw-w64-ucrt-x86_64-texlive-fonts-recommended \
  mingw-w64-ucrt-x86_64-texlive-formats-extra \
  mingw-w64-ucrt-x86_64-texlive-humanities \
  mingw-w64-ucrt-x86_64-texlive-lang-czechslovak \
  mingw-w64-ucrt-x86_64-texlive-lang-english \
  mingw-w64-ucrt-x86_64-texlive-lang-european \
  mingw-w64-ucrt-x86_64-texlive-latex-extra \
  mingw-w64-ucrt-x86_64-texlive-latex-recommended \
  mingw-w64-ucrt-x86_64-texlive-luatex \
  mingw-w64-ucrt-x86_64-texlive-metapost \
  mingw-w64-ucrt-x86_64-texlive-pictures \
  mingw-w64-ucrt-x86_64-texlive-plain-generic \
  mingw-w64-ucrt-x86_64-texlive-pstricks \
  mingw-w64-ucrt-x86_64-texlive-publishers \
  mingw-w64-ucrt-x86_64-texlive-science
```
Nakoniec, aby sme mohli používať `pandoc` z interpretera Bash, je potrebné pridať nasledujúci riadok:
```Bash
export PATH="/c/Users/mrena/AppData/Local/Pandoc:$PATH"
```
na koniec súboru `~/.bashrc` (`/c/msys64/home/{UPN}/.bashrc`). Môžeme to spraviť, napríklad, príkazom:
```Bash
echo "export PATH=\"/c/Users/mrena/AppData/Local/Pandoc:\$PATH\""
```
PDF dokument s dokumentáciou vytvoríme spustením nasledujúceho príkazu:
```Bash
pandoc documentation.markdown --from markdown+lists_without_preceding_blankline --to pdf --template=template.tex --output documentation.pdf
```
v priečinku s dokumentáciou (`cd doc/semestral_project`).





# Úniky pamäte
Na kontrolu únikov pamäte v semestrálnej práci použite knižnicu `heap-monitor`, ktorá sa nachádza medzi závislosťami projektu. Pre jej použitie je potrebné linkovať cieľ `heap-monitor` v súbore `semestral_project/CMakeLists.txt`:
```cmake
target_link_libraries(semestral_project PRIVATE ds heap-monitor)
```
Pre aktivovanie kontroly je potrebné do každého **`.cpp`** súboru ako ako posledný príkaz include pridať:
```cpp
#include <heap/heap-monitor.hpp>
```
Knižnica potom eviduje všetky použitia operátorov `new` a `delete`.

## Hlavičkové súbory
Ak máte implementáciu funkcií (či už z dôvodu použitia šablón, optimalizácie alebo nerozvážnej organizácie zdrojového kódu) v hlavičkových súboroch, je potrebné aktivovať kontrolu úniku pamäte použitím direktívy:
```cpp
#include <heap/header-begin.hpp>

```
na začiatku súboru a direktívy:
```cpp
#include <heap/header-end.hpp>
```
na konci súboru. Príklad použitia sa nachádza v súbore `dummy.hpp`.
