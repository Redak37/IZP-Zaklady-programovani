/*
 *  Projekt 2 - Iteracni vypocty
 *  Autor: Radek Duchon
 *  Datum: 19.11.2017
 *  Nazev souboru: proj2.c
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

enum {ERROR = 1, HELP, TAN, CM};

//zjisti, jestli je dany retezec cislo
int isnumber(char *retezec)
{
    char *endptr;
    double number = strtod(retezec, &endptr);

    if(*endptr == '\0' && endptr != retezec && !isnan(number))
        return 1;
    else
        return 0;
}
//zjisti, jestli je dany retezec cele cislo
int celecislo(char *retezec)
{
    char *endptr;
    strtol(retezec, &endptr, 10);

    if (*endptr == '\0' && endptr != retezec)
        return 1;
    else
        return 0;
}
//vypise napovedu
void napoveda()
{
    printf("Napoveda:\n");
    printf("Program je mozno spustit s argumenty --help, --tan a -m\n");
    printf("1) Argument --help spousti napovedu.\n");
    printf("2) Argument --tan porovnava presnosti vypoctu tangens uhlu mezi tan z matematicke knihovny, vypoctem\n");
    printf("   pomoci zretezenych zlomku a pomoci Taylorova polynomu.\n");
    printf(" - Je nutno zadat uhel v radianech (inf a nan nejsou validni vstupy) a nasledne kolikate iterace maji byt porovnavany\n");
    printf(" - Interval se zadava dvoumi celociselnymi argumenty od 1 do 13.\n");
    printf("3) Argument -m spocita vysku a vzdalenost objektu.\n");
    printf(" - Je nutno zadat uhel alfa v radianech (uhel pohledu ke spodni casti objektu v intervalu (0; 1.4>).\n");
    printf(" - Je mozno zadat argument -c s hodnotami (0, 100> pro nastaveni vysky pohledu mericiho prostroje, implicitne 1.5 m.\n");
    printf(" - Je mozno zadat uhel beta v radianech (uhel pohledu k vrchni casti objektu) v intervalu (0; 1.4> pro spocitani vysky objektu.\n");
}
//vypocet tangens pomoci taylorovy funkce
double taylor_tan(double x, unsigned int n)
{
    unsigned long long citatel[] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
    unsigned long long jmenovatel[] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    double vysledek = x, vyraz = x * x;

    for (unsigned short int i = 1; i < n; ++i)
    {
        x *= vyraz;
        vysledek += x * citatel[i] / jmenovatel[i];
    }

    return vysledek;
}
//vypocet tangens pomoci zretezenych zlomku
double cfrac_tan(double x, unsigned int n)
{
    unsigned short int citatel_ve_jmenovateli = (2 * n) + 1;
    double vysledek = 0; 

    for (unsigned int i = 0; i < n; ++i)
    {
        citatel_ve_jmenovateli -= 2;
        vysledek = 1 / ((citatel_ve_jmenovateli / x) - vysledek);
    }

    return vysledek;
}
//Urcuje vykonavanou ulohu a zjisti vetsinu neplatnych vstupu
int vstup(int argc, char *argv[])
{
    if (argc == 1 || argc > 6){}

    else if(strcmp(argv[1], "--help") == 0 && argc == 2)
        return HELP;

    else if ((strcmp(argv[1], "-m") == 0 || strcmp(argv[1], "-c") == 0) && argc > 2)
    {
        for (unsigned short int i = 2; i < argc; ++i)
        {
            if ((strcmp(argv[i - 1], "-m") == 0 || strcmp(argv[i - 1], "-c") == 0) && !isnumber(argv[i]))
                return ERROR;

            if (strcmp(argv[i - 2], "-m") == 0 && strcmp(argv[i], "-c") != 0 && !isnumber(argv[i]))
                return ERROR;

            if ((strcmp(argv[i], "-c") == 0 && argc != (i + 2)) || (strcmp(argv[i], "-m") == 0 && (argc != (i + 2) && argc != (i + 3))))
                return ERROR;
        }
        return CM;
    
    }else if (strcmp(argv[1], "--tan") == 0 && argc == 5)
        if (isnumber(argv[2]) && celecislo(argv[3]) && celecislo(argv[4]))
            if (1 <= atoi(argv[3]) && atoi(argv[3]) <= atoi(argv[4]) && atoi(argv[4]) <= 13)
                if(!isinf(atof(argv[2])))
                    return TAN;

    return ERROR;
}
//porovnava zjistene hodnoty s tangens z matematicke knihovny
int srovnani(double radian, unsigned int n, unsigned int m)
{
    double tay, zlom;

    for (unsigned int i = n; i <= m; ++i)
    {
        tay = taylor_tan(radian, i);
        zlom = cfrac_tan(radian, i);
        printf("%d %e %e %e %e %e\n", i, tan(radian), tay, fabs(tay - tan(radian)), zlom, fabs(zlom - tan(radian)));
    }

    return 0;
}
//vypocita vzdalenost, pripadne vysku objektu pomoci zadanych udaju
int vzdalenost (double vyska, double alfa, double beta)
{   //kontrolue nepovolene hodnoty parametru pro tan
    if (vyska <= 0 || vyska > 100 || alfa <= 0 || alfa > 1.4 || beta < 0 || beta > 1.4)
        return ERROR;
    //pri zvysujicim se uhlu roste odchylka, pro max uhel 1.4 staci 9 iteraci pro presnost na deset mist
    double vzdal = vyska / cfrac_tan(alfa, 9);

    printf("%.10e\n", vzdal);

    if (beta != 0)
        printf("%.10e\n", cfrac_tan(beta, 9) * vzdal + vyska);

    return 0;
}
//urcuje vysku, alfu a pripadne betu a posila udaje k dalsimu vypoctu
int cm(int argc, char *argv[])
{
    double vyska = 1.5, alfa, beta = 0;

    for (unsigned short int i = 2; i < argc; ++i)
    {
        if (strcmp(argv[i - 1], "-m") == 0)
            alfa = atof(argv[i]);

        else if (strcmp(argv[i - 1], "-c") == 0)
            vyska = atof(argv[i]);

        else if (strcmp(argv[i - 2], "-m") == 0 && strcmp(argv[i], "-c") != 0)
        {
            alfa = atof(argv[i - 1]);
            beta = atof(argv[i]);

            if (atof(argv[i]) == 0)
                return ERROR;
        }
    }

    if (vzdalenost(vyska, alfa, beta) == ERROR)
        return ERROR;

    return 0;
}

int main(int argc, char *argv[])
{   //rozhoduje se, ktera cast programu se spusti
    switch (vstup(argc, argv)){
    case HELP:
        napoveda();
        break;
    case TAN:
        srovnani(atof(argv[2]), atof(argv[3]), atoi(argv[4]));
        break;
    case CM:
        if (cm(argc, argv) != ERROR)
            break;
    default:
        fprintf(stderr, "Error, zadany neplatne parametry, pro napovedu spustte program s argumentem --help\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
