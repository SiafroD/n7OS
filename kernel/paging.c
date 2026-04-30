#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include "string.h"
#include <n7OS/processor_structs.h>

// #include "stdio.h"

PageDirectory pageDirectory;

extern uint32_t placement_address;

void findPageTableAtAddress(uint32_t address, PageTable* _table, PTE** pte) {

    PageTable table = (PageTable)(pageDirectory[address >> 22].value & 0xFFFFF000);

    if (_table) {
        *_table = table;
    }

    if (pte) {
        *pte = table + ((address & 0x3FF000) >> 12);
    }
}


void setPageEntry(PTE *page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
    page_table_entry->page_entry.P = 1;
    page_table_entry->page_entry.A = 0;
    page_table_entry->page_entry.D = 0;
    page_table_entry->page_entry.W = is_writeable;
    page_table_entry->page_entry.U = ~is_kernel;
    page_table_entry->page_entry.Page = new_page>>12;
}

void initialise_paging() {

    kmalloc_init();

    init_mem();

    pageDirectory = (PageDirectory) kmalloc_a(sizeof(PDE)*NB_ENTREES_DIRECTORY);
    memset(pageDirectory, 0, sizeof(PDE)*NB_ENTREES_DIRECTORY);

    for (uint16_t i = 0; i < NB_ENTREES_DIRECTORY ; i++) {
        PageTable new_page_table = (PageTable) kmalloc_a(sizeof(PTE)*NB_ENTREES_PAGE);
        memset(new_page_table, 0, sizeof(PTE)*NB_ENTREES_PAGE);
        pageDirectory[i].value= ((uint32_t)new_page_table) | 0b11;
    }

    for (uint32_t address = 0 ; address<placement_address ; address+= PAGE_SIZE) {
        alloc_page_entry(address, 1, 1);
    }

    setup_base((int)pageDirectory);

    __asm__ __volatile__("mov %0, %%cr3" :: "r"(pageDirectory));


    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));
    cr0 = cr0 | 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0" : : "r"(cr0));
    


}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab;
    PTE* pte;

    findPageTableAtAddress(address, &pgtab, &pte);
    pte->page_entry.P = 1;
    pte->page_entry.W = is_writeable;
    pte->page_entry.U = ~is_kernel;
    pte->page_entry.D = 0;
    pte->page_entry.A = 0;
    pte->page_entry.Page = findfreePage() >> 12;

    return pgtab;
}
