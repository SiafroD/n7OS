#include <n7OS/mem.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <string.h>

static uint32_t premiere_page;

static bitmap_t bitmap;

// nombre de bits par entrée de bitmap (égal à TAILLE_GROUPES)
static const uint32_t bits_per_entry = TAILLE_GROUPES;

/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    uint32_t group = page_index / bits_per_entry;
    uint32_t offset = page_index % bits_per_entry;
    bitmap[group] = bitmap[group] | (1u << offset);
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    uint32_t page_index = addr / PAGE_SIZE;
    uint32_t group = page_index / bits_per_entry;
    uint32_t offset = page_index % bits_per_entry;
    bitmap[group] = bitmap[group] & ~(1u << offset);
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    uint32_t address = 0;

    for (uint16_t GID = 0 ; GID < NOMBRE_GROUPES ; GID++) {
        for (uint8_t PID_Group = 0 ; PID_Group < TAILLE_GROUPES ; PID_Group++ ) {
            if(!((bitmap[GID] >> PID_Group) & 0x1)) {
                address = (GID * TAILLE_GROUPES + PID_Group) * PAGE_SIZE;
                setPage(address);
                return address;
            }
        }
    } 
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    bitmap = (bitmap_t) kmalloc(NOMBRE_GROUPES * sizeof(uint32_t));
    memset(bitmap, 0, NOMBRE_GROUPES * sizeof(uint32_t));
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    for(uint16_t group_id = 0; group_id < NOMBRE_GROUPES; group_id++) {
        for(uint8_t page_id_in_group = 0; page_id_in_group < TAILLE_GROUPES; page_id_in_group++) {
            printf("%u : %u | ", group_id * TAILLE_GROUPES + page_id_in_group,
                   (bitmap[group_id] >> page_id_in_group) & 0x1);
        }
        printf("\n");
    }
}