#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>

#if defined __i386__
#define ASM_HOOK_CODE "\x68\x00\x00\x00\x00\xc3" // push 0x00000000, ret
#define ASM_HOOK_CODE_OFFSET 1
#elif defined __x86_64__
#define ASM_HOOK_CODE "\x48\xb8\x00\x00\x00\x00\x00\x00\x00\x00\xff\xe0" // mov rax 0x0000000000000000, jmp rax
#define ASM_HOOK_CODE_OFFSET 2
#else
#error ARCH_ERROR_MESSAGE
#endif

static void *original_devmem_function = NULL;
static char original_function_code_backup[sizeof(ASM_HOOK_CODE) - 1];

int modified_devmem_function(unsigned long pagenr)
{
    return 1;
}

int init_module(void)
{
    original_devmem_function = (void *)kallsyms_lookup_name("devmem_is_allowed");
    if (!original_devmem_function)
    {
        printk(KERN_INFO "Can't locate the needed symbol, is it exported?\n");
        return -1;
    }

    preempt_disable();
    write_cr0(read_cr0() & (~0x10000)); //disable write-protected memory

    memcpy(original_function_code_backup, original_devmem_function, sizeof(ASM_HOOK_CODE) - 1);
    memcpy(original_devmem_function, ASM_HOOK_CODE, sizeof(ASM_HOOK_CODE) - 1);
    *(void **)&((char *)original_devmem_function)[ASM_HOOK_CODE_OFFSET] = modified_devmem_function;

    write_cr0(read_cr0() | 0x10000); // enable write-protected memory
    preempt_enable();

    printk(KERN_INFO "Memory access module loaded\n");
    return 0;
}

void cleanup_module(void)
{
    preempt_disable();
    write_cr0(read_cr0() & (~0x10000)); //disable write-protected memory

    memcpy(original_devmem_function, original_function_code_backup, sizeof(ASM_HOOK_CODE) - 1);

    write_cr0(read_cr0() | 0x10000); // enable write-protected memory
    preempt_enable();

    printk(KERN_INFO "Memory access module unloaded\n");
}

MODULE_AUTHOR("Ozgun Ayaz");
MODULE_DESCRIPTION("make /dev/mem writable beyond the first 1 MB");
MODULE_LICENSE("GPL");
MODULE_INFO(intree, "Y"); //bullshit lol
