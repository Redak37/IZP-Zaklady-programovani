/**
 * Projekt 3 - Jednoducha shlukova analyza
 * Autor: Radek Duchon
 * Datum: 10.12.2017
 * Nazev souboru: proj3.c
 */

/**
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

enum {ERROR, AVG, MIN, MAX};

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO
    if (cap < 0)
        c->capacity = 0;
    else
        c->capacity = cap;

    c->size = 0;

    if (cap > 0)
    {
        c->obj = malloc(cap * sizeof(struct obj_t));
        if (c->obj == NULL)
            c->capacity = -1;
    }else
        c->obj = NULL;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO
    free(c->obj);
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;
short premium_case = AVG;
/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO
    if (c->size == c->capacity)
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);

    c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    for (int i = 0; i < c2->size; ++i)
        append_cluster(c1, c2->obj[i]);

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    clear_cluster(&carr[idx]);
    
    for (int i = idx; i < narr - 1; ++i)
        carr[i] = carr[i+1];

    return --narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    return sqrtf(((o1->x - o2->x) * (o1->x - o2->x)) + ((o1->y - o2->y) * (o1->y - o2->y)));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float distance = 0.0;

    if (premium_case == AVG)
    {
        for (int i = 0; i < c1->size; ++i)
            for (int j = 0; j < c2->size; ++j)
                distance += obj_distance(&c1->obj[i], &c2->obj[j]);
    
        return distance / (c1->size * c2->size);
    }
    else if (premium_case == MIN)
    {
        float min_distance = obj_distance(&c1->obj[0], &c2->obj[0]);
        
        for (int i = 0; i < c1->size; ++i)
            for (int j = 0; j < c2->size; ++j)
            {
                distance = obj_distance(&c1->obj[i], &c2->obj[j]);
                if (distance < min_distance)
                    min_distance = distance;
            }

        return min_distance;
    }
    else// if (premium_case == MAX)
    {
        float max_distance = obj_distance(&c1->obj[0], &c2->obj[0]);

        for (int i = 0; i < c1->size; ++i)
            for (int j = 0; j < c2->size; ++j)
            {
                distance = obj_distance(&c1->obj[i], &c2->obj[j]);
                if (distance > max_distance)
                    max_distance = distance;
            }

        return max_distance;
    }
}


/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    // TODO
    float distance, min_distance = cluster_distance(&carr[0], &carr[1]);
    for (int i = 0; i < narr; ++i)
        for (int j = i + 1; j < narr; ++j)
        {
            distance = cluster_distance(&carr[i], &carr[j]);
            
            if (distance <= min_distance)
            {
                *c1 = i;
                *c2 = j;
                min_distance = distance;
            }
        }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
//Načte shluky, alokuje si pro ne pamet a vrati pocet nactenych shluku.
//V ptipade chyby pamet uvolni, vypise chybu na stderr a vrati hodnotu -1
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    // TODO
    FILE *file = fopen(filename, "r");
    int count, x, y;
    //Chyba, pokud se nepodarilo nacist soubor
    if (file == NULL)
    {
        arr = NULL;
        fprintf(stderr, "Error, soubor %s se nepodarilo nacist\n", filename);
        return -1;
    }

    if (fscanf(file, "count=%d", &count))
    {   //Chyba, pokud se ma cist 0 a mene objektu
        if (count <= 0)
        {
            fclose(file);
            arr = NULL;
            fprintf(stderr, "Error, Zadano chybne mnozstvi objektu\n");
            return -1;
        }
        if (fgetc(file) !='\n')
        {
            fclose(file);
            arr = NULL;
            fprintf(stderr, "Error, Zadano chybne mnozstvi objektu\n");
            return -1;
        }

        *arr = malloc(count * sizeof(struct cluster_t));

        if (*arr == NULL)
        {
            fclose(file);
            arr = NULL;
            fprintf(stderr, "Error, nepodarilo se alokovat pamet\n");
            return -1;
        }
        //Nacitani objektu a alokace pameti pro shluky
        for (int i = 0; i < count; ++i)
        {
            init_cluster(&arr[0][i], 1);

            if (arr[0][i].capacity == -1)
            {
                fprintf(stderr, "Error, nepodarilo se alokovat pamet\n");
                count = -1;
            }
            //Zjistovani chyb nacitani ze souboru
            if (fscanf(file, "%d %d %d", &arr[0][i].obj[0].id, &x, &y) != 3){
                fprintf(stderr, "Error, chybny format souboru\n");
                count = -1;
            }else if (fgetc(file) !='\n'){
                fprintf(stderr, "Error, chybny format souboru\n");
                count = -1;
            }else{
            arr[0][i].obj[0].x = x;
            arr[0][i].obj[0].y = y;
            }
            
            if (count != -1){
            if (x < 0 || x > 1000 || y < 0 || y > 1000){
                fprintf(stderr, "Error, neplatne souradnice %d. objektu\n", i);
                count = -1;
            }else
                arr[0][i].size++;
            //Kontrola ID
            for (int j = 0; j < i; ++j)
                if (arr[0][i].obj[0].id == arr[0][j].obj[0].id)
                {
                    fprintf(stderr, "Error, shodujici se ID %d. a %d. objektu\n", i, j);
                    count = -1;
                }
            }
            //odstaneni alokovane pameti pri chybe
            if (count == -1)
            {
                for (int j = 0; j <= i; ++j)
                    if (arr[0][j].capacity != -1)
                        clear_cluster(&(arr[0][j]));

                free(*arr);
                arr = NULL;
            }
        }
    }else{
        arr = NULL;
        count = -1;
        fprintf(stderr, "Error, chybny format zadani poctu objektu\n");
    }

    fclose(file);
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
 
    // TODO
    // Promene pro pocet shluku, pozadovany pocet shluku a indexy
    int num, count = 1, idx_1, idx_2;
    //zjisteni pozadovaneho poctu shluku, vpripade chyby vraci 0
    if (argc >= 3)
    {
        char *ptr;
        count = strtol(argv[2], &ptr, 10);

        if (*ptr != '\0')
            count = 0;
    }
    //Urceni metody shlukovani
    if (argc == 4)
    {
        if (strcmp(argv[3], "--avg") == 0)
            premium_case = AVG;
        else if (strcmp(argv[3], "--min") == 0)
            premium_case = MIN;
        else if (strcmp(argv[3], "--max") == 0)
            premium_case = MAX;
        else
            premium_case = ERROR;
    }

    if (argc == 1 || argc > 4 || count <= 0 || premium_case == 0) 
    {
        fprintf(stderr, "Error, zadany neplatne argumenty\n");
        return EXIT_FAILURE;
    }

    num = load_clusters(argv[1], &clusters);

    if (num < count && num != -1)
        fprintf(stderr, "Error, zadan prilis vysoky pocet shluku\n");
    //Slucovani shluku do pozadovaneho poctu
    while (count < num)
    {
        find_neighbours(clusters, num, &idx_1, &idx_2);
        merge_clusters(&clusters[idx_1], &clusters[idx_2]);
        remove_cluster(clusters, num, idx_2);
        --num;
    }
    //zaznaceni chyby,  pokud je pozadovany pocet shluku vyssi nez dany pocet
    if (count > num || num == -1)
        count = -1;

    for (int i = 0; i < count; ++i)
        sort_cluster(&clusters[i]);

    if (count != -1)
        print_clusters(clusters, count);
    //uvolneni shluku
    while (num > 0)
    {
        --num;
        clear_cluster(&clusters[num]);
    }

    if (num == 0)
        free(clusters);

    return (count == -1) ? EXIT_FAILURE : EXIT_SUCCESS;
}
