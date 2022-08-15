#include "fs.h"
#include "kheap.h"
#include "string.h"

static int vfs_open(struct file *f) {
  if (f == NULL) {
    return -1;
  }
  return 0;
}

static size_t vfs_read(struct file *f, char *buf, size_t len) {
  if (f->inode == NULL) {
    return -1;
  }
  char *text = "hello world";
  uintptr_t off = (uintptr_t)text + f->offset;
  f->offset += len;
  memcpy(buf, (void *)off, len);
  return len;
}

struct file_operations vfs_file_operations = {
    .open = vfs_open,
    .read = vfs_read,
};

struct inode *inode_lookup(struct inode *d, char *name, size_t name_len) {
  if (d == NULL) {
    return NULL;
  }
  if (strncmp(name, "test.txt", name_len) != 0) {
    return NULL;
  }
  struct inode *i = kmalloc(sizeof(struct inode));
  *i = (struct inode){
      .id = 0x01,
      .operations = &vfs_inode_operations,
  };
  return i;
}
struct inode_operations vfs_inode_operations = {
    .lookup = inode_lookup,
};

struct inode root_node = {.id = 1, .operations = &vfs_inode_operations};

// allocates file and inode
struct file *open(char *path) {
  // find file
  struct inode *i = root_node.operations->lookup(&root_node, path, 8);
  if (i == NULL) {
    return NULL;
  }

  struct file *file = kmalloc(sizeof(struct file));
  *file = (struct file){
      .inode = i,
      .operations = &vfs_file_operations,
  };
  file->operations->open(file);
  return file;
}

size_t read(struct file *file, char *buf, size_t len) {
  if (file->operations->read == NULL) {
    return -1;
  }
  size_t read = file->operations->read(file, buf, len);
  return read;
}
