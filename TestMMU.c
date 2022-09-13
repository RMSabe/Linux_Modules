//A Test code for MMU module

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "MMU_usr.h"

int main(int argc, char **argv)
{
	printf("Started\n");
	if(!mmu_init())
	{
		printf("MMU Init Error\nTerminated\n");
		return 0;
	}
	printf("MMU Tool Initialized\n");

	printf("\nAnalysis:\n\n");

	uint32_t number = 69420;
	printf("Var value: %d\n", number);

	uint64_t phys_addr = mmu_get_phys_from_virt(&number);
	printf("Var physical addr: 0x%lx\n", phys_addr);

	void *pvoid = mmu_get_virt_from_phys(phys_addr);
	printf("Var virtual addr: %p\n", pvoid);

	printf("\nDebug:\n\n");
	
	printf("Var virtual addr: %p\n", &number);

	uint32_t value = *((uint32_t*) pvoid);
	printf("Var value: %d\n", value);

	printf("\n");
	printf("Terminated\n");
	return 0;
}
