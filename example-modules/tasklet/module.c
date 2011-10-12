/* This example shows usage of work queues. Both normal work and
 * delayed work are demonstrated. The type my_work_t is used for both.
 * When a work is actually performed, it output the value of the
 * member x.
 */

#include "linux/module.h"
#include "linux/interrupt.h"
#include "linux/kernel.h"
#include "linux/slab.h"

#include "proc_info.h"

#define FAILED_HERE() printk(KERN_ALERT "clli:Failed %s:%d.\n", \
                             __FILE__, __LINE__)

static int tasklet_module_major = 60;
static char *tasklet_module_name = "tasklet_module";
static unsigned long tasklet_data = 123;

static int tasklet_module_init(void);
static void tasklet_module_exit(void);
static void tasklet_function(unsigned long data);

MODULE_AUTHOR("Claes Lillieskold");
MODULE_DESCRIPTION("An example of tasklet usage.");
MODULE_LICENSE("GPL");
MODULE_PARM_DESC(tasklet_module_major, "Module template major number");
MODULE_PARM_DESC(tasklet_module_name, "Module template name");

module_exit(tasklet_module_exit);
module_init(tasklet_module_init);
module_param(tasklet_module_major, int, S_IRUGO);
module_param(tasklet_module_name, charp, S_IRUGO);

DECLARE_TASKLET(tasklet_example, tasklet_function,
                (unsigned long)&tasklet_data);

static void tasklet_function(unsigned long data)
{
        printk(KERN_ALERT "Now running the tasklet_function(%ld).\n",
               *(unsigned long *)data);
}

static int tasklet_module_init(void)
{
        /* Announce the beginning of example and log process state. */
        printk(KERN_ALERT "clli: tasklet example init.\n");
        proc_info("The init function");

        /* Schedule the tasklet to run. */
        tasklet_schedule(&tasklet_example);
        
	return 0;
}
  
static void tasklet_module_exit(void)
{
        printk(KERN_ALERT "clli: tasklet example exits.\n");
        proc_info("The exit function");

        /* Remove the tasklet scheduling. */
        tasklet_kill(&tasklet_example);
}
