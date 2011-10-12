#include "asm/current.h"
#include "linux/module.h"
#include "linux/sched.h"

#include "proc_info.h"

void log_proc_info(void)
{
        printk(KERN_ALERT "clli: Running in process with pid:0x%08lx.\n",
               (unsigned long)current->pid);
}

EXPORT_SYMBOL(log_proc_info);
