Údaje s ktorými pracujete v semestrálnom projekte môžete umiestniť do tohto priečinku.

Keďže sú súbory pomerne veľké, nebudeme ich commitovať do repozitárov. Všetky `.csv` súbory v tomto priečinku sú preto ignorované.

Dáta pochádzajú z datasetu *Road Safety Data* (Department for Transport) a stiahnuť ich môžete tu: <https://www.data.gov.uk/dataset/cb7ae6f0-4be6-4935-9277-47e5ce24a11f/road-accidents-safety-data> (súbory `dft-road-casualty-statistics-collision-YYYY.csv`).

V nasledujúcej ukážke kódu môžete vidieť, ako so súbormi pracovať z vášho kódu. Lepšie by však bolo, ak by bola cesta k súboru argumentom vášho programu.

```C++
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream ifst("../semestral_project/data/dummy.txt");
    std::string line;
    while (std::getline(ifst, line)) {
        std::cout << line << "\n";
    }
}
```
