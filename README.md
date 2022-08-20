# [IZP] Základy programování - projekty

2017/18 - 1. semestr

# Projekt 1 - Práce s textem - 5/5 bodů

  Popis projektu
  Cílem projektu je vytvořit program, který by emuloval algoritmus výpočtu povolení a zakázání kláves na virtuální klávesnici navigace. Vstupem programu jsou data známých adres a uživatelem zadaný řetězec. Výstup programu bude obsahovat znaky, které mají být na klávesnici povoleny.

  Detailní specifikace
  Program implementujte ve zdrojovém souboru proj1.c. Vstupní data budou čtena ze standardního vstupu (stdin), výstup bude tisknut na standardní výstup (stdout).

  Překlad a odevzdání zdrojového souboru
  Odevzdání: Odevzdejte zdrojový soubor proj1.c prostřednictvím informačního systému.

  Překlad: Program překládejte s následujícími argumenty

  $ gcc -std=c99 -Wall -Wextra -Werror proj1.c -o proj1
  Syntax spuštění
  Program se spouští v následující podobě: (./proj1 značí umístění a název programu):

  ./proj1 ADRESA
  Pokud je program spuštěn bez argumentů, bere zadanou adresu jako prázdný řetězec.

  Implementační detaily
  Vstupní databáze adres
  Databáze adres jsou textová data, u kterých každý řádek označuje jednu adresu. Každý řádek obsahuje maximálně 100 znaků. Seznam adres je neuspořádaný. U všech dat nezáleží na velikosti písmen (tzv. case insensitive). Program musí podporovat alespoň 42 adres.

  Výstup programu
  Výstup programu může být trojího druhu:

  adresa nalezena,
  adresa vyžaduje specifikaci,
  adresa nenalezena.
  Adresa nalezena
  Found: S
  Tento výstup se tiskne, pokud je v databázi adres nalezena jediná adresa S, jejíž prefix odpovídá uživatelem zadané adrese ADRESA. (Pozn. prefix P řetězce S je takový řetězec, u kterého řetězec S začíná řetězcem P).

  Adresa vyžaduje specifikaci
  Enable: CHARS
  Pokud je v databázi adres nalezeno více adres odpovídající danému prefixu ADRESA, program pomocí takto naformátovaného řádku vytiskne seznam povolených kláves CHARS. CHARS je abecedně seřazený seznam znaků, u nichž pro každý znak C platí, že v databázi adres existuje adresa, jejíž prefix odpovídá spojení řetězce ADRESA s daným znakem C.

  Adresa nenalezena
  Not found
  Pokud v databázi adres neexistuje adresa, jejíž prefix by odpovídal zadanému řetězci ADRESA, vytiskne program toto hlášení.

  Omezení v projektu
  Je zakázané použít následující funkce:

  volání z rodiny malloc a free - práce s dynamickou pamětí není v tomto projektu zapotřebí,
  volání z rodiny fopen, fclose, fscanf, ... - práce se soubory (dočasnými) není v tomto projektu žádoucí,
  volání qsort, lsearch, bsearch a hsearch - cílem je zamyslet se nad algoritmizací a strukturou dat.
  

# Projekt 2 - Iterační výpočty - 7/7 bodů
  
  Zadání projektu
  Implementujte funkce nutné pro výpočet vzdálenosti a výšky pomocí úhlu náklonu měřeného přístroje. Výpočet proveďte pouze pomocí matematických operací +,-,*,/. Implementujte výpočet vzdálenosti a výšky měřeného objektu.

  Překlad a odevzdání zdrojového souboru
  Odevzdání: Program implementujte ve zdrojovém souboru proj2.c. Zdrojový soubor odevzdejte prostřednictvím informačního systému.

  Překlad: Program překládejte s následujícími argumenty:

  $ gcc -std=c99 -Wall -Wextra -Werror proj2.c -lm -o proj2
  Syntax spuštění
  Program se spouští v následující podobě:

  ./proj2 --help
  nebo
  ./proj2 --tan A N M
  nebo
  ./proj2 [-c X] -m A [B]
  Argumenty programu:

  --help způsobí, že program vytiskne nápovědu používání programu a skončí.
  --tan porovná přesnosti výpočtu tangens úhlu A (v radiánech) mezi voláním tan z matematické knihovny, a výpočtu tangens pomocí Taylorova polynomu a zřetězeného zlomku. Argumenty N a M udávají, ve kterých iteracích iteračního výpočtu má porovnání probíhat. 0 < N <= M < 14
  -m vypočítá a změří vzdálenosti.
  Úhel α (viz obrázek) je dán argumentem A v radiánech. Program vypočítá a vypíše vzdálenost měřeného objektu. 0 < A <= 1.4 < π/2.
  Pokud je zadán, úhel β udává argument B v radiánech. Program vypočítá a vypíše i výšku měřeného objektu. 0 < B <= 1.4 < π/2
  Argument -c nastavuje výšku měřicího přístroje c pro výpočet. Výška c je dána argumentem X (0 < X <= 100). Argument je volitelný - implicitní výška je 1.5 metrů.
  Implementační detaily
  Je zakázané použít funkce z matematické knihovny.
  
 
# Projekt 3 - Jednoduchá shluková analýza - 10/10 bodů + 2 body prémie
	    - Dokumentace - 3.3/4 body
  Popis projektu
  Vytvořte program, který implementuje jednoduchou shlukovou analýzu, metodu "Unweighted pair-group average" (pouze anglicky).

  Shlukovou analýzu provádějte na dvourozměrných objektech. Každý objekt je identifikován celým číslem. Objekty jsou uloženy v textovém souboru.

  Detailní specifikace
  Překlad a odevzdání zdrojového souboru
  Odevzdání: Program implementujte ve zdrojovém souboru proj3.c. Zdrojový soubor odevzdejte prostřednictvím informačního systému.

  Překlad: Program bude překládán s následujícími argumenty

  $ gcc -std=c99 -Wall -Wextra -Werror -DNDEBUG proj3.c -o proj3 -lm
  Definice makra NDEBUG (argument -DNDEBUG) je z důvodu anulování efektu ladicích informací.
  Propojení s matematickou knihovnou (argument -lm) je z důvodu výpočtu vzdálenosti objektů.
  Syntax spuštění
  Program se spouští v následující podobě:

  ./proj3 SOUBOR [N]
  Argumenty programu:

  SOUBOR je jméno souboru se vstupními daty.
  N je volitelný argument definující cílový počet shluků. N > 0. Výchozí hodnota (při absenci argumentu) je 1.
  Implementační detaily
  Formát vstupního souboru
  Vstupní data jsou uložena v textovém souboru. První řádek souboru je vyhrazen pro počet objektů v souboru a má formát:
    count=N
    kde číslo je počet objektů v souboru. Následuje na každém řádku definice jednoho objektu. Počet řádků souboru odpovídá minimálně počtu objektů + 1 (první řádek). Další řádky souboru ignorujte. Řádek definující objekt je formátu:
    OBJID X Y
    kde OBJID je v rámci souboru jednoznačný celočíselný identifikátor, X a Y jsou souřadnice objektu také celá čísla. Platí 0 <= X <= 1000, 0 <= Y <= 1000.



2020

# Projekt 3 - Průchod bludištěm - zhotoveno za minimum času pouze pro výukové potřeby
