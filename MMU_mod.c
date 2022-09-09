//Memory Mapping Utility Module File.

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <asm/io.h>

/*
 * Memory Mapping Utils Command Structure:
 *
 * 3 UINTS:
 * UINT0:
 * BYTE0 (BITS 31-24): CMD
 * BYTE1 (BITS 23-16): ARG0
 * BYTE2 (BITS 15-8): ARG1
 * BYTE3 (BITS 7-0): ARG2
 *
 * UINT 1-2 (1 ULONG)
 * UINT1 (ULONG HIGH BITS): ARG ADDR HIGH 32 BITS
 * UINT2 (ULONG LOW BITS): ARG ADDR LOW 32 BITS
 */

#define MMU_DATAIO_SIZE_BYTES 12
#define MMU_DATAIO_SIZE_UINT 3

#define MMU_CMD_GET_PHYSICAL_ADDR 1
#define MMU_CMD_GET_VIRTUAL_ADDR 2

static struct proc_dir_entry *mmu_proc = NULL;
static unsigned int *mmu_data_io = NULL;

static unsigned long mmu_get_physical_from_virtual(unsigned long virtual_addr)
{
	return virt_to_phys((void*) virtual_addr);
}

static unsigned long mmu_get_virtual_from_physical(unsigned long physical_addr)
{
	return (unsigned long) phys_to_virt(physical_addr);
}

ssize_t mmu_mod_usrread(struct file *file, char __user *user, size_t size, loff_t *offset)
{
	copy_to_user(user, mmu_data_io, MMU_DATAIO_SIZE_BYTES);
	return size;
}

ssize_t mmu_mod_usrwrite(struct file *file, const char __user *user, size_t size, loff_t *offset)
{
	copy_from_user(mmu_data_io, user, MMU_DATAIO_SIZE_BYTES);
	unsigned char *pbyte = (unsigned char*) &mmu_data_io[0];
	unsigned long *pulong = (unsigned long*) &mmu_data_io[1];

	unsigned int cmd = pbyte[0];
	unsigned int arg0 = pbyte[1];
	unsigned int arg1 = pbyte[2];
	unsigned int arg2 = pbyte[3];

	unsigned long input_addr = *pulong;
	unsigned long output_addr = 0;

	switch(cmd)
	{
		case MMU_CMD_GET_PHYSICAL_ADDR:
			output_addr = mmu_get_physical_from_virtual(input_addr);
			*pulong = output_addr;
			break;

		case MMU_CMD_GET_VIRTUAL_ADDR:
			output_addr = mmu_get_virtual_from_physical(input_addr);
			*pulong = output_addr;
			break;
	}

	return size;
}

static const struct file_operations mmu_proc_ops = {
	.read = mmu_mod_usrread,
	.write = mmu_mod_usrwrite
};

static int __init driver_enable(void)
{
	mmu_proc = proc_create("MMU", 0x1B6, NULL, &mmu_proc_ops);
	if(mmu_proc == NULL)
	{
		printk("Error Creating MMU Proc File\n");
		return -1;
	}

	mmu_data_io = (unsigned int*) vmalloc(MMU_DATAIO_SIZE_BYTES);
	printk("MMU Tool Enabled\n");
	return 0;
}

static void __exit driver_disable(void)
{
	proc_remove(mmu_proc);
	vfree(mmu_data_io);
	printk("MMU Tool Disabled\n");
	return;
}

module_init(driver_enable);
module_exit(driver_disable);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rafael Sabe");
MODULE_DESCRIPTION("Virtual-Physical memory mapping utility tool for GNU-Linux");
