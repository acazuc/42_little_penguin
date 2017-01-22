#ifndef FILE_JIFFIES_H
# define FILE_JIFFIES_H

ssize_t jiffies_read(struct file *fp, char __user *data, size_t len
		, loff_t *off);

#endif
