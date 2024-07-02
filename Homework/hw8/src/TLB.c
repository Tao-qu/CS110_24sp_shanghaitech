#include "../inc/TLB.h"

#include <stdlib.h>

unsigned global_TLB_init(void) {
  global_tlb = calloc(1, sizeof(TLB));
  return 0;
}

void global_TLB_free(void) {
  if (global_tlb == NULL) {
    return;
  }
  free(global_tlb);
  global_tlb = NULL;
}

unsigned read_TLB(proc_id_t pid, unsigned vpn) {
  if (global_tlb->pid != pid) {
    flush_TLB(pid);
    return -1;
  }

  for (size_t i = 0; i < TLB_SIZE; ++i) {
    if (global_tlb->entries[i].valid == 1 &&
        global_tlb->entries[i].vpn == vpn) {
      global_tlb->entries[i].lut = global_tlb->clock;
      ++global_tlb->clock;
      return global_tlb->entries[i].ppn;
    }
  }
  return -1;
}

void write_TLB(proc_id_t pid, unsigned vpn, unsigned ppn) {
  if (global_tlb->pid != pid) {
    flush_TLB(pid);
  }

  uint32_t min_lut = -1;
  size_t lru_index = -1;
  for (size_t i = 0; i < TLB_SIZE; ++i) {
    if (global_tlb->entries[i].valid == 0) {
      lru_index = i;
      break;
    }
    if (global_tlb->entries[i].lut < min_lut) {
      min_lut = global_tlb->entries[i].lut;
      lru_index = i;
    }
  }
  global_tlb->entries[lru_index].vpn = vpn;
  global_tlb->entries[lru_index].ppn = ppn;
  global_tlb->entries[lru_index].valid = 1;
  global_tlb->entries[lru_index].lut = global_tlb->clock;
  ++global_tlb->clock;
}

void remove_TLB(proc_id_t pid, unsigned vpn) {
  if (global_tlb->pid != pid) {
    return;
  }

  for (size_t i = 0; i < TLB_SIZE; ++i) {
    if (global_tlb->entries[i].valid && global_tlb->entries[i].vpn == vpn) {
      global_tlb->entries[i].valid = 0;
      return;
    }
  }
}

void flush_TLB(proc_id_t pid) {
  global_tlb->pid = pid;
  global_tlb->clock = 0;
  for (size_t i = 0; i < TLB_SIZE; ++i) {
    global_tlb->entries[i].valid = 0;
  }
}
