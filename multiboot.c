#include <stddef.h>

#include "multiboot.h"

struct multiboot_mod_list *get_multiboot_module(struct multiboot_info *ptr,
                                                size_t module_index) {
  if ((ptr->flags & MULTIBOOT_INFO_MODS) == 0) {
    return NULL;
  }
  struct multiboot_mod_list *_addrs =
      (struct multiboot_mod_list *)ptr->mods_addr;
  return &_addrs[module_index];
}
