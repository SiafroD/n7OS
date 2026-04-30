/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

extern void loadPageDirectory(unsigned int* dir);
extern void enablePaging();

#define PAGE_PRESENT 1
#define PAGE_RW 2
#define PAGE_USER 4
#define PAGE_DIRTY 16

#define NB_ENTREES_PAGE 1024
#define NB_ENTREES_DIRECTORY 1024
#define NB_PAGES (NB_ENTREES_PAGE*NB_ENTREES_DIRECTORY)

/**
 * @brief Description d'une ligne de la table de page
 * 
 */
typedef struct {
    uint32_t P : 1; // Page présente en mémoire
    uint32_t W : 1; // Page accessible en lecture/écriture : 0 pour lecture
    uint32_t U : 1; // Page utilisateur : 0 pour noyau
    uint32_t A : 1; // Accessed bit
    uint32_t D : 1; // Dirty bit (page modifiée)
    uint32_t RSVD_AVAIL : 7; // bits réservés
    uint32_t Page : 20; // Adresse de la page en mémoire physique

} page_table_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief Une table de page (PageTable) est un tableau de descripteurs de page
 * 
 */
typedef PTE * PageTable;

/**
 * @brief Description d'une ligne du répertoire de page
 * 
 */

typedef struct {
    uint32_t P : 1;
    uint32_t W : 1;
    uint32_t U : 1;
    uint32_t RSVD : 9;
    uint32_t Page : 20;
} page_directory_entry_t;

/**
 * @brief Une entrée dans le répertoire de page peut être manipulée en utilisant
 *        la structure page_directory_entry_t ou directement la valeur
 */
typedef union {
    page_directory_entry_t page_entry;
    uint32_t value;
} PDE;

/**
 * @brief Un répertoire de page (PageDirectory) est un tableau de descripteurs
 *        de page
 */
typedef PDE * PageDirectory;

extern PageDirectory pageDirectory;

/**
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 *        et active la pagination
 * 
 */
void initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif