/**
 *@file
 *
 *@mainpage
 *
 *@brief Jednoducha shlukova analyza
 *
 *@author Radek Duchon
 *
 *@date 15.12.2017
 */

 /**
 * @defgroup cluster_array_function Array
 *
 * @brief funkce pracujici s polem shluku
 */

 /**
 * @defgroup cluster_function Cluster
 *
 * @brief funkce pracujici se shluky
 */

 /**
 * @struct obj_t
 *
 * @brief struktura tvorici objekt
 *
 * Objekt je tvoren jednoznacnym ID a souradnicemi X, Y.
 */

/**
 * @struct obj_t
 *
 * @brief struktura tvorici objekt
 *
 * Objekt je tvoren jednoznacnym ID a souradnicemi X, Y.
 */
struct obj_t {
    int id; /**< identifikator objektu */
    float x; /**< Souradnice X objektu */
    float y; /**< Souradnice Y objektu */
};
/**
 * @struct cluster_t
 *
 * @brief struktura tvorici shluk
 *
 * Struktura je tvorena velikosti, kapacitou a ukazatelem na pole objektu.
 */
struct cluster_t {
    int size; /**< Soucasna velikost shluku */
    int capacity; /**< Maximalni kapacita shluku */
    struct obj_t *obj; /**< Ukazatel na pole objektu */
};
/**
 * @ingroup cluster_function
 *
 * @brief inicializace shluku 'c'
 *
 * Alokuje pamet pro kapacitu 'cap' shluku 'c'.
 *
 * @param c ukazatel na shluk
 * @param cap kapacita inicializovaneho shluku
 *
 * @pre c != NULL
 *
 * @post c->size == 0
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @ingroup cluster_function
 *
 * @brief uvolneni shluku
 *
 * Dealokuje pamet shluku a inicializuje jej na kapacitu 0.
 *
 * @param c ukazatel na shluk
 *
 * @post c->size == 0
 * @post c->capacity == 0
 * @post c->obj == NULL
 */
void clear_cluster(struct cluster_t *c);

/**
 *@brief konstanta doporucena pro rozsirovani shluku
 */
extern const int CLUSTER_CHUNK;

/**
 * @ingroup cluster_function
 *
 * @brief realokace kapacity shluku
 *
 * Zvysuje kapacitu shluku 'c' na kapacitu 'new_cap'
 *
 * @param c ukazatel na shluk
 * @param new_cap velikost po zvetseni
 *
 * @pre c != NULL
 * @pre c->capacity >= 0
 * @pre new_cap >= 0
 *
 * @return vraci shluk 'c'
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @ingroup cluster_function
 *
 * @brief rozsireni shluku
 *
 * Rozsiri shluk 'c' o objekt 'obj'.
 *
 * @param c ukazatel na shluk
 * @param obj objekt k zarazeni do shluku
 *
 * @post c->size += 1
 * @post c->capacity >= c->size
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @ingroup cluster_function
 *
 * @brief slouceni shluku
 *
 * Prida shluk 'c2' do shluku 'c1'.
 * V pripade nutnosti zvysi kapacitu shluku 'c1'.
 *
 * @param c1 ukazatel na rozsirovany shluk
 * @param c2 ukazatel na rozsirujici shluk
 *
 * @pre c1 != NULL
 * @pre c2 != NULL
 *
 * @post c1->size += c2->size
 * @post c1->capacity >= c1->size
 * @post c1->obj[i].id < c1->obj[i+1].id
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @ingroup cluster_array_function
 *
 * @brief odstraneni shluku
 *
 * Odstrani shluk na pozisici idx z pole shluku carr.
 *
 * @param carr ukazatel na pole shluku
 * @param narr pocet shluku v poli
 * @param idx index ostranovaneho shluku
 *
 * @pre idx < narr
 * @pre narr > 0
 *
 * @return vraci novy pocet shluku (narr - 1)
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @ingroup cluster_function
 *
 * @brief vzdalenost objektu
 *
 * Vypocita euklidovskou vzdalenost dvou objektu.
 *
 * @param o1 ukazatel na objekt 1
 * @param o2 ukazatel na objekt 2
 *
 * @pre o1 != NULL
 * @pre o2 != NULL
 *
 * @return vraci vzdalenost dvou objektu
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @ingroup cluster_function
 *
 * @brief vzdalenost shluku
 *
 * Vypocita euklidovskou vzdalenost dvou shluku.
 *
 * @param c1 ukazatel na shluk 1
 * @param c2 ukazatel na shluk 2
 *
 * @pre c1 != NULL
 * @pre c1->size > 0
 * @pre c2 != NULL
 * @pre c2->size > 0
 *
 * @return vraci vzdalenost dvou shluku
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @ingroup cluster_array_function
 *
 * @brief hledani sousedu
 *
 * Najde 2 nejblizsi shluky v poli shluku.
 *
 * @param carr ukazatel na pole shluku
 * @param narr pocet shluku v poli 'carr'
 * @param c1
 * @param c2
 *
 * @pre narr > 0
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @ingroup cluster_function
 *
 * @brief serazeni shluku
 *
 * Seradi objekty ve shluku podle ID.
 *
 * @param c ukazatel na shluk objektu
 *
 * @post c->obj[i].id < c->obj[i+1].id
 */
void sort_cluster(struct cluster_t *c);

/**
 * @ingroup cluster_function
 *
 * @brief tisk shluku
 *
 * Vytiskne shluk objektu
 *
 * @param c ukazatel na shluk objektu
 *
 * @pre narr > 0
 */
void print_cluster(struct cluster_t *c);

/**
 * @ingroup cluster_array_function
 *
 * @brief nacteni objektu
 *
 * Ze souboru 'filename' nacte objekty do samostatnych shluku, pro ktere alokuje misto.
 *
 * @param filename jmeno souboru ze ktereho se maji nacitat objekty
 * @param arr ukazatel na ukazatel na pole shluku
 *
 * @pre arr != NULL
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @ingroup cluster_array_function
 *
 * @brief tisk shluku
 *
 * Vytiskne shluky
 *
 * @param carr ukazatel na pole shluku
 * @param narr pocet tisknutych shluku
 */
void print_clusters(struct cluster_t *carr, int narr);
