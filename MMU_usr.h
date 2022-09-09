//Memory Mapping Utility

#ifndef MMU_H
#define MMU_H

#include <stdbool.h>
#include <stdint.h>

bool mmu_is_active(void);
bool mmu_init(void);
uint64_t mmu_get_phys_from_virt(void *virtaddr);
void *mmu_get_virt_from_phys(uint64_t physaddr);

#endif
