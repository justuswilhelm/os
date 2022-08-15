#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "kheap.h"

typedef uint32_t ino_t;

struct inode {
  ino_t id;
  struct inode_operations *operations;
};

struct inode_operations {
  // TODO
  // lookup
  struct inode *(*lookup)(struct inode *, char *, size_t);
};

// Represent an open file
struct file {
  struct inode *inode;
  struct file_operations *operations;
  size_t offset;
};

struct file_operations {
  int (*open)(struct file *);
  size_t (*read)(struct file *, char *, size_t);
  // TODO
  // read
  // write
  // iterate, iterate over all directory members
};

// https://elixir.bootlin.com/linux/v4.6/source/fs/libfs.c#L144
// iterate_dir

extern struct inode_operations vfs_inode_operations;
extern struct file_operations vfs_file_operations;

struct file *open(char *);
size_t read(struct file *, char *, size_t);
