#include "linux/module.h"
#include "linux/kernel.h"
#include "linux/workqueue.h"
#include "linux/slab.h"

#define FAILED_HERE() printk(KERN_ALERT "Failed %s:%d.\n", __FILE__, __LINE__)

static int work_queue_module_init(void);
static void work_queue_module_exit(void);

static int work_queue_module_major = 60;
static char *work_queue_module_name = "work_queue_module";

MODULE_AUTHOR("Claes Lillieskold");
MODULE_DESCRIPTION("An example of workqueue usage.");
MODULE_LICENSE("GPL");

module_param(work_queue_module_major, int, S_IRUGO);
MODULE_PARM_DESC(work_queue_module_major, "Module template major number");
module_param(work_queue_module_name, charp, S_IRUGO);
MODULE_PARM_DESC(work_queue_module_name, "Module template name");

module_init(work_queue_module_init);
module_exit(work_queue_module_exit);

struct workqueue_struct *my_wq;

typedef struct
{
        struct work_struct my_work;
        int x;
} my_work_t;

my_work_t *work, work2;

static void my_eq_function(struct work_struct *work)
{
        my_work_t *my_work = (my_work_t *)work;

        printk("my_work.x = %d.\n", my_work->x);
        kfree((void*) work);
}

static int add_work_to_queue(struct workqueue_struct *queue, my_work_t *work,
                             void (func)(struct work_struct *), int x)
{
        int ret;
        
        INIT_WORK((struct work_struct*)work, func);
        work->x = x;
        ret = queue_work(queue, (struct work_struct*)work);

        if (ret == 0)
        {
                printk(KERN_ALERT "The work %p already added.\n", work);
                FAILED_HERE();
                return __LINE__;
        }

        return 0;
}

static int work_queue_module_init(void)
{
        int ret;

        /* Create a work queue first. */
        my_wq = create_workqueue("my_queue");
        if (!my_wq)
        {
                printk(KERN_ALERT "Failed to create work queue.\n");
                FAILED_HERE();
                return __LINE__;
        }

        /* Create the first work. */
	printk(KERN_ALERT "Clli work_queue example init.\n");
        work = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
        if (!work)
        {
                printk(KERN_ALERT "kmalloc failed.\n");
                FAILED_HERE();
                return __LINE__;
        }

        /* Queue the first work. */
        ret = add_work_to_queue(my_wq, work, my_eq_function, 1);
        if (ret != 0)
        {
                return __LINE__;
        }
        
	return 0;
}
  
static void work_queue_module_exit(void)
{
}

