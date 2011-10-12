#include "asm/current.h"
#include "linux/module.h"
#include "linux/sched.h"

#include "proc_info.h"

void proc_info(const char *who)
{
        if (who == NULL)
        {
                who = "Unknown";
        }
        
        printk(KERN_ALERT "clli: %s runs in pid:0x%08lx.\n",
               who, (unsigned long)current->pid);
}
