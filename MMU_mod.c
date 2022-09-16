//Memory Mapping Utility Module

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <asm/io.h>

/*
 * MMU Command Structure (9 BYTES):
 *
 * BYTE0: CMD
 *
 * BYTES 1-8 (1 ULONG): 64BIT ADDR
 */

#define MMU_DATAIO_SIZE_BYTES 9

#define MMU_CMD_GET_PHYSICAL_ADDR 1
#define MMU_CMD_GET_VIRTUAL_ADDR 2

static struct proc_dir_entry *mmu_proc = NULL;
static void *mmu_data_io = NULL;

unsigned long mmu_get_physical_addr(unsigned long virtual_addr)
{
	return virt_to_phys((void*) virtual_addr);
}

unsigned long mmu_get_virtual_addr(unsigned long physical_addr)
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
	unsigned char *pbyte = (unsigned char*) mmu_data_io;
	unsigned long *pulong = (unsigned long*) &pbyte[1];

	switch(pbyte[0])
	{
		case MMU_CMD_GET_PHYSICAL_ADDR:
			pulong[0] = mmu_get_physical_addr(pulong[0]);
			break;

		case MMU_CMD_GET_VIRTUAL_ADDR:
			pulong[0] = mmu_get_virtual_addr(pulong[0]);
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
		printk("MMU: Error Creating MMU Proc File\n");
		return -1;
	}

	mmu_data_io = vmalloc(MMU_DATAIO_SIZE_BYTES);
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
MODULE_DESCRIPTION("Memory Mapping Utility Tool for GNU-Linux Systems.");
