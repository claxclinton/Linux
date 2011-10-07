#include "linux/init.h"
#include "linux/module.h"
#include "linux/kernel.h"

static int clli_example_module_init(void);
static void clli_example_module_exit(void);

static int clli_example_module_major = 60;
static char *clli_example_module_name = "clli_example_module";

MODULE_AUTHOR("Claes Lillieskold");
MODULE_DESCRIPTION("Claes example module");
MODULE_LICENSE("GPL");

module_param(clli_example_module_major, int, S_IRUGO);
MODULE_PARM_DESC(clli_example_module_major, "Module template major number");
module_param(clli_example_module_name, charp, S_IRUGO);
MODULE_PARM_DESC(clli_example_module_name, "Module template name");

module_init(clli_example_module_init);
module_exit(clli_example_module_exit);

static int clli_example_module_init(void)
{
  printk(KERN_ALERT "Clli example module init.\n");
  return 0;
}
  
static void clli_example_module_exit(void)
{
}

