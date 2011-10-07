#include "linux/init.h"
#include "linux/module.h"
#include "linux/kernel.h"

static int do_nothing_module_init(void);
static void do_nothing_module_exit(void);

static int do_nothing_module_major = 60;
static char *do_nothing_module_name = "do_nothing_module";

MODULE_AUTHOR("Claes Lillieskold");
MODULE_DESCRIPTION("Claes example module");
MODULE_LICENSE("GPL");

module_param(do_nothing_module_major, int, S_IRUGO);
MODULE_PARM_DESC(do_nothing_module_major, "Module template major number");
module_param(do_nothing_module_name, charp, S_IRUGO);
MODULE_PARM_DESC(do_nothing_module_name, "Module template name");

module_init(do_nothing_module_init);
module_exit(do_nothing_module_exit);

static int do_nothing_module_init(void)
{
  printk(KERN_ALERT "Clli example module init.\n");
  return 0;
}
  
static void do_nothing_module_exit(void)
{
}

