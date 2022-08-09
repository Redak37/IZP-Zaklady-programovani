/*
 *  Projekt 1 - Prace s textem
 *  Autor: Radek Duchon
 *  Datum: 5.11.2017
 *  Nazev souboru: proj1.c
 */

#include <stdio.h>
#define MAX 102//maximalni delka mesta + znak konce radku + znak konce retezce

//Prepise male pismeno na velke, zaroven zkontroluje, jestli se jedna o tisknutelny znak, pokud ne, vrati EOF
char ToUpper(char znak)
{
    return (znak >= 'a' && znak <= 'z') ? znak + 'A' - 'a' : -1;
}

//Vraci delku retezce
int GetLength(char *Slovo)
{
    unsigned short i = 0;
    while(Slovo[i] != '\0' && Slovo[i] != '\n')
        ++i;
    return i;
}

//Zjisti, jestli se jiz znak nachazi v retezci, pokud ano, vrati 1, jinak vrati 0
int porovnej(char *retezec, char nove)
{
    unsigned short j = GetLength(retezec);
    for (unsigned short i = 0; i < j; ++i)
        if (retezec[i] == nove)
            return 1;
    return 0;
}

//Zaradi novy znak dle ASCII do retezce, pokud se v nem jeste nenachazi
void zarad(char *retezec, char nove)
{
    if (!porovnej(retezec, nove))
    {
        unsigned short DelkaRetezce = GetLength(retezec);
        retezec[DelkaRetezce] = nove;

        for (unsigned short i = 0; i < DelkaRetezce; ++i)
        {
            if (retezec[i] > nove)
            {
                for (unsigned short j = DelkaRetezce; j > i; --j)
                    retezec[j] = retezec[j - 1];
                retezec[i] = nove;
                break;
            }
        }
    }
}

//Prepise pole do jineho pole
void PoleDoPole(char *odkud, char *kam)
{
    unsigned short i;
    for (i = 0; odkud[i]; ++i)
        kam[i] = odkud[i];
    kam[i] = odkud[i];
}


int main (int argc, char *argv[])
{   //Pole pro ukladani mest, povolenych znaku a vyhledavaciho retezce
    char city[MAX] = {0}, mesto[MAX] = {0}, enable[95] = {0}, vyhledavani[MAX] = {0};
    //Promenne pro urceni rovnosti s argumentem a poctu nalezenych zaznamu
    unsigned short rovnost, nalezeno = 0, NalezenoArgv = 0;

    if (argc > 2)
        fprintf(stderr, "Warning: Zadano prilis mnoho argumentu, bude pouzit prvni argument\n");
    //Pokud byl zadan argument, prepise ho do retezce vyhledavani, prepise mala pismena na velka a zkontroluje platnost znaku
    if (argc > 1)
        for (unsigned short i = 0; argv[1][i] != 0 && i < MAX - 1; ++i)
        {
            vyhledavani[i] = ToUpper(argv[1][i]);

            if (vyhledavani[i] == -1)
            {
                fprintf(stderr, "Error: Zadan neplatny znak v argumentu, konec programu\n");
                return 1;
            }
        }
    //Konstanta zaznamenavajici delku argumentu
    const unsigned short delka = GetLength(vyhledavani);
    //Nacita postupne adresy do retezce city ze stdin
    for (unsigned i = 1; fgets(city, MAX, stdin); ++i)
    {
        rovnost = 1;
        //Zjisťuje, jestli se dana adresa vejde do maxima znaku, kontroluje jejich platnost, v pripade chyby vypise chybove hlaseni
        for (unsigned short j = 0; city[j] != '\n'; ++j)
        {
            if (j == MAX - 2)
            {
                fprintf(stderr, "Warning: Adresa na %d. radku je prilis dlouha, nadbytecne znaky budou ignorovany\n", i);
                city[MAX - 2] = '\n';
                while(getchar() != '\n'){}
                break;
            }

            if (city[j] != '\n')
                city[j] = ToUpper(city[j]);

            if (city[j] == -1)
            {
                fprintf(stderr, "Warning: %d. znak v adrese na %d. radku je neplatny, adresa bude preskocena\n", j + 1, i);
                rovnost = 0;
            }
        }
        //Porovnava retezec vyhledavani s prvnimi znaky retezce city a urcuje jejich rovnost
        for (unsigned short j = 0; j < delka; ++j)
            if (vyhledavani[j] != city[j])
                rovnost = 0;

        if (rovnost)
        {
            if (++nalezeno == 1)
                PoleDoPole(city, mesto);
            //Pokud city obsahuje vice znaku, nez vyhledavaci retezec, zaradi novy znak do pole enable podle ASCII hodnoty
            if (city[delka] != '\0' && city[delka] != '\n')
                zarad(enable, city[delka]);
            else
                ++NalezenoArgv;
        }
    }

    if (NalezenoArgv > 0 && vyhledavani[0] != '\0')
        printf("Found: %s\n", vyhledavani);

    if (nalezeno == 0 && NalezenoArgv == 0)
        printf("Not found\n");
    else if (nalezeno == 1 && NalezenoArgv == 0)
        printf("Found: %s\n", mesto);
    else if ((NalezenoArgv > 0 && nalezeno > NalezenoArgv) || nalezeno - NalezenoArgv > 1)
        printf("Enable: %s\n", enable);

    return 0;
}
