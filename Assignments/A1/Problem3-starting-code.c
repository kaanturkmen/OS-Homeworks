#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include<linux/slab.h>

struct fileinfo304 {
	size_t size;
	char filename[100];
        char datecreated[100];
	int owner_id;
        int file_id; 
	struct list_head list;
};

/**
 * The following defines and initializes a list_head object named files_list
 */

static LIST_HEAD(files_list);


int fileinfo304_init(void)
{
       printk(KERN_INFO "Loading Module\n");

       return 0;
}

void fileinfo304_exit(void) {

	printk(KERN_INFO "Removing Module\n");
}

module_init( fileinfo304_init);
module_exit( fileinfo304_exit);

MODULE_LICENSE( "GPL");
MODULE_DESCRIPTION( "Exercise for COMP304");
MODULE_AUTHOR("Your Name");

