#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/livepatch.h>

#include <linux/seq_file.h>


int modified_devmem_function(unsigned long pagenr)
{
    return 1;
}

static struct klp_func funcs[] = {
    {
        .old_name = "devmem_is_allowed",
        .new_func = modified_devmem_function,
    }, { }
};

static struct klp_object objs[] = {
    {
        /* name being NULL means vmlinux */
        .funcs = funcs,
    }, { }
};

static struct klp_patch patch = {
    .mod = THIS_MODULE,
    .objs = objs,
};

static int livepatch_init(void)
{
    return klp_enable_patch(&patch);
}

static void livepatch_exit(void)
{
}

module_init(livepatch_init);
module_exit(livepatch_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("make /dev/mem writable beyond the first 1 MB");
MODULE_INFO(livepatch, "Y");
