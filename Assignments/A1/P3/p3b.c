#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/types.h>

// Creating a processID which will be inputed by the user.
int processID = -1;

// 0644 is a permission that making file readable by every user group but written by user only.
module_param(processID, int, 0644);

// Adding definition to our parameter.
MODULE_PARM_DESC(processID, "Process ID which should be given by the user.");


int process_comp304_init(void) {

	// Creating struct pointers for further use.
	struct task_struct *initTaskStruct;
	struct list_head *initListHead;
	struct task_struct *ts;

	// If user gives 0 or less as a parameter, printing an error and terminating.
	if (processID <= 0) {
		printk(KERN_INFO "ERROR: Process ID is invalid. Terminating.\n");
		return -1;
	} else {
		printk(KERN_INFO "Loading the module.\n");

		// Getting task_struct struct from the process id.
		ts = pid_task(find_vpid(processID), PIDTYPE_PID);

		// If something wrong happens while gathering task struct, printing an error and terminating.
		if (ts == NULL) {
			printk(KERN_INFO "p3b: Could not find process with the given pid. %d\n", processID);
			return -2;
		}

		// Process ID
		printk(KERN_INFO "p3b: Process ID = %d\n", processID);

		// Parent's Process ID.
		printk(KERN_INFO "p3b: Parent Process ID: %d\n", task_ppid_nr(ts));

		// Executable Name.
		printk(KERN_INFO "p3b: Executable name of given PID: %s\n", ts->comm);

		// Looping through each children of our task struct, then getting and printing list entry of sibling.
		list_for_each(initListHead, &ts->children) {
			initTaskStruct = list_entry(initListHead, struct task_struct, sibling);
			printk(KERN_INFO "p3b: Sibling ID: %d, Sibling Executable Name: %s\n", initTaskStruct->pid, initTaskStruct->comm);
		}

	}

	return 0;
}
void process_comp304_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

// Defining init and exit functions.
module_init(process_comp304_init);
module_exit(process_comp304_exit);

// Module information.
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Exercise for COMP304");
MODULE_AUTHOR("Kaan Turkmen");
