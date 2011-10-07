#include "linux/module.h"
#include "linux/kernel.h"
#include "linux/workqueue.h"
#include "linux/slab.h"

/* This example shows usage of work queues. Both normal work and
 * delayed work are demonstrated. The type my_work_t is used for both.
 * When a work is actually performed, it output the value of the
 * member x.
 */
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
        union
        {
                struct work_struct ws;
                struct delayed_work dw;
        };
        int x;
} my_work_t;

my_work_t *work1;
my_work_t *work2;
my_work_t *work3_delayed;

static void log_work_info(struct work_struct *work)
{
        my_work_t *my_work = (my_work_t *)work;

        printk("my_work.x = %d.\n", my_work->x);
        kfree((void*) work);
}

static int create_work_struct(my_work_t **work_out,
                              void (func)(struct work_struct *), int x)
{
        my_work_t *work;

        /* Allocate memory for work. */
        work = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
        if (!work)
        {
                printk(KERN_ALERT "Failed to alloc a my_work_t.\n");
                FAILED_HERE();
                *work_out = NULL;
                return __LINE__;
        }
        
        INIT_WORK((struct work_struct*)work, func);
        work->x = x;
        *work_out = work;

        return 0;
}

static int add_work_to_queue(struct workqueue_struct *queue,
                             my_work_t **work_out,
                             void (func)(struct work_struct *), int x)
{
        int ret;

        /* Allocate create and init work struct. */
        ret = create_work_struct(work_out, func, x);
        if (ret != 0)
        {
                return ret;
        }
        
        ret = queue_work(queue, (struct work_struct*)*work_out);
        if (ret == 0)
        {
                printk(KERN_ALERT "The work %p already added.\n", *work_out);
                FAILED_HERE();
                return __LINE__;
        }

        return 0;
}

static int create_delayed_work(my_work_t **work_out,
                               void (func)(struct work_struct*), int x)
{
        my_work_t *work;

        /* Allocate memory for work. */
        work = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
        if (!work)
        {
                printk(KERN_ALERT "Failed to alloc a my_work_t.\n");
                FAILED_HERE();
                *work_out = NULL;
                return __LINE__;
        }
        
        INIT_DELAYED_WORK(((struct delayed_work*)work), func);
        work->x = x;
        *work_out = work;

        return 0;
}

static int add_work_to_queue_delayed(struct workqueue_struct *queue,
                                     my_work_t **work_out,
                                     void (func)(struct work_struct*), int x,
                                     unsigned long delay)
{
        int ret;

        /* Allocate create and init work struct. */
        ret = create_delayed_work(work_out, func, x);
        if (ret != 0)
        {
                return ret;
        }
        
        ret = queue_delayed_work(queue, (struct delayed_work*)*work_out, delay);
        if (ret == 0)
        {
                printk(KERN_ALERT "The work %p already added.\n", *work_out);
                FAILED_HERE();
                return __LINE__;
        }

        return 0;
}

static int work_queue_module_init(void)
{
        int ret;

        printk(KERN_ALERT "Clli work_queue example init.\n");

        /* Create a work queue first. */
        my_wq = create_workqueue("my_queue");
        if (!my_wq)
        {
                printk(KERN_ALERT "Failed to create work queue.\n");
                FAILED_HERE();
                return __LINE__;
        }

        /* Queue the first work. */
        ret = add_work_to_queue(my_wq, &work1, log_work_info, 1);
        if (ret != 0)
        {
                return ret;
        }
        
        /* Queue the second work. */
        ret = add_work_to_queue(my_wq, &work2, log_work_info, 2);
        if (ret != 0)
        {
                return ret;
        }
        
        /* Queue the the third work delayed. */
        ret = add_work_to_queue_delayed(my_wq, &work3_delayed, log_work_info,
                                        3, 100);
        if (ret != 0)
        {
                return ret;
        }
        
	return 0;
}
  
static void work_queue_module_exit(void)
{
        printk(KERN_ALERT "Clli work_queue example exits.\n");
        destroy_workqueue(my_wq);
}
