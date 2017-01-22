#ifndef FILE_FOO_H
# define FILE_FOO_H

ssize_t foo_write(struct file *fp, const char __user *data, size_t len
		, loff_t *off);
ssize_t foo_read(struct file *fp, char __user *data, size_t len
		, loff_t *off);

#endif
