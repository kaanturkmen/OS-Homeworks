#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>

// Struct of fileinfo304.
struct fileinfo304 {
	size_t size;
	char filename[100];
	char datecreated[100];
	int owner_id;
	int file_id;
	struct list_head list;
};

// The following defines and initializes a list_head object named files_list
static LIST_HEAD(files_list);

// Init function which runs when module is loaded into the kernel.
int fileinfo304_init(void) {
	// Creating struct fileinfo304 pointer for further use.
	struct fileinfo304 *initFile;
	int i;

	printk(KERN_INFO "Loading Module\n");

	// Looping 5 times.
	for (i = 0; i < 5; i++) {

		// Allocating memory for our newly defined pointer.
		struct fileinfo304 *file = kmalloc(sizeof(*file), GFP_KERNEL);

		// Assigning values to it,
		file->size = 30 * (i + 1);

		strcpy(file->filename, "Hello, COMP304!");
		strcpy(file->datecreated, "Spring 2021");

		file->owner_id = 72213;
		file->file_id = 314 * (i + 1);

		// Initiliazing its list data structure.
		INIT_LIST_HEAD(&file->list);

		// Adding the Initiliazed list to end of our linked list.
		list_add_tail(&file->list, &files_list);
	}

	list_for_each_entry(initFile, &files_list, list) {
		// Printing each element of the list.
		printk(KERN_INFO
				"File size: %zu, File name: %s, File Creation Date: %s, Owner ID: "
				"%d, File ID: %d\n",
				initFile->size, initFile->filename, initFile->datecreated,
				initFile->owner_id, initFile->file_id);
	}

	return 0;
}

// Exit function which runs when module is unloaded from the kernel.
void fileinfo304_exit(void) {
	struct fileinfo304 *firstFile, *nextFile;

	printk(KERN_INFO "Removing Module\n");

	list_for_each_entry_safe(firstFile, nextFile, &files_list, list) {
		list_del(&firstFile->list);
		kfree(firstFile);
	}
}

// Defining init and exit functions.
module_init(fileinfo304_init);
module_exit(fileinfo304_exit);

// Module information.
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Exercise for COMP304");
MODULE_AUTHOR("Kaan Turkmen");
