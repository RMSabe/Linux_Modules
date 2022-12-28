# Linux_Modules_64bit

!!DEPRECATED!! Look For My "GNU-Linux_KernelModule_MMU" repository.

General Purpose Modules for 64bit GNU-Linux systems.

Here I post Linux Kernel Modules that I made. This specific repository is meant for Linux 64bit Kernel, though it should run on Linux 32bit Kernel as well.

Files:

"Makefile" and "_mod.c" files are module files. 
Unless you want to understand in depth how these modules work, you won't need to worry about understanding these files.
You will just need to compile them using the kernel headers to generate the kernel object ".ko" files, and then install those.

"_usr.c" and "_usr.h" files are user files.
They are the header and source files used by the userspace executable to interact/control the module.

Resources:
To compile the module file, you will need the kernel headers.

Useful commands (PS: I use Debian based distro. Some of these commands might be exclusive for Debian based GNU-Linux distros):

Use "sudo apt install linux-headers-$(uname -r)" to install the kernel headers.
Use "sudo insmod <file.ko>" to install a module.
Use "sudo rmmod <file.ko>" to uninstall a module.
Use "lsmod" to list installed modules.

I'm not a professional software developer. I make these codes just for fun. Don't expect professional performance from them.

Author: Rafael Sabe
Email: rafaelmsabe@gmail.com
