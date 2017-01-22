#ifndef FILE_ID_H
# define FILE_ID_H

ssize_t id_write(struct file *fp, const char __user *data, size_t len
		, loff_t *off);
ssize_t id_read(struct file *fp, char __user *data, size_t len
		, loff_t *off);

#endif
