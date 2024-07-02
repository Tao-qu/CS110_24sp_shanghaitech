#include "../inc/simulator.h"
#include "../inc/TLB.h"
#include <assert.h>
#include <stdlib.h>

status_t allocate_page(Process *process, addr_t address,
                       addr_t physical_address) {
  // This is guaranteed by the simulator
  assert(address >> OFFSET_BITS << OFFSET_BITS == address);
  assert(physical_address >> OFFSET_BITS << OFFSET_BITS == physical_address);
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }
  // TODO: Implement me!
  unsigned L1_index =
      (address >> (L2_BITS + OFFSET_BITS)) & (L1_PAGE_TABLE_SIZE - 1);
  unsigned L2_index = (address >> OFFSET_BITS) & (L2_PAGE_TABLE_SIZE - 1);

  if (process->page_table.entries[L1_index].entries == NULL) {
    process->page_table.entries[L1_index].entries =
        (PTE *)calloc(L2_PAGE_TABLE_SIZE, sizeof(PTE));
    if (process->page_table.entries[L1_index].entries == NULL) {
      return ERROR;
    }
  }

  if (process->page_table.entries[L1_index].entries[L2_index].valid != 0) {
    return ERROR;
  }

  process->page_table.entries[L1_index].entries[L2_index].frame =
      physical_address >> OFFSET_BITS;
  process->page_table.entries[L1_index].entries[L2_index].valid = 1;
  ++process->page_table.entries[L1_index].valid_count;

  return SUCCESS;
}

status_t deallocate_page(Process *process, addr_t address) {
  // This is guaranteed by the simulator
  assert(address >> OFFSET_BITS << OFFSET_BITS == address);
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }
  // TODO: Implement me!
  unsigned L1_index =
      (address >> (L2_BITS + OFFSET_BITS)) & (L1_PAGE_TABLE_SIZE - 1);
  unsigned L2_index = (address >> OFFSET_BITS) & (L2_PAGE_TABLE_SIZE - 1);

  if (process->page_table.entries[L1_index].entries == NULL ||
      process->page_table.entries[L1_index].entries[L2_index].valid == 0) {
    return ERROR;
  }

  process->page_table.entries[L1_index].entries[L2_index].valid = 0;
  --process->page_table.entries[L1_index].valid_count;
  if (process->page_table.entries[L1_index].valid_count == 0) {
    free(process->page_table.entries[L1_index].entries);
    process->page_table.entries[L1_index].entries = NULL;
  }
  remove_TLB(process->pid, address >> OFFSET_BITS);

  return SUCCESS;
}

status_t read_byte(Process *process, addr_t address, byte_t *byte) {
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }
  // TODO: Implement me!
  unsigned ppn = read_TLB(process->pid, address >> OFFSET_BITS);
  if (ppn != (unsigned)-1) {
    *byte = main_memory->pages[ppn]->data[address & (PAGE_SIZE - 1)];
    return TLB_HIT;
  }

  unsigned L1_index =
      (address >> (L2_BITS + OFFSET_BITS)) & (L1_PAGE_TABLE_SIZE - 1);
  unsigned L2_index = (address >> OFFSET_BITS) & (L2_PAGE_TABLE_SIZE - 1);

  if (process->page_table.entries[L1_index].entries == NULL ||
      process->page_table.entries[L1_index].entries[L2_index].valid == 0) {
    return ERROR;
  }

  ppn = process->page_table.entries[L1_index].entries[L2_index].frame;
  *byte = main_memory->pages[ppn]->data[address & (PAGE_SIZE - 1)];
  write_TLB(process->pid, address >> OFFSET_BITS, ppn);

  return SUCCESS;
}

status_t write_byte(Process *process, addr_t address, const byte_t *byte) {
  // 1. Check if the process is valid
  if (process == NULL) {
    return ERROR;
  }
  // TODO: Implement me!
  unsigned ppn = read_TLB(process->pid, address >> OFFSET_BITS);
  if (ppn != (unsigned)-1) {
    main_memory->pages[ppn]->data[address & (PAGE_SIZE - 1)] = *byte;
    return TLB_HIT;
  }

  unsigned L1_index =
      (address >> (L2_BITS + OFFSET_BITS)) & (L1_PAGE_TABLE_SIZE - 1);
  unsigned L2_index = (address >> OFFSET_BITS) & (L2_PAGE_TABLE_SIZE - 1);

  if (process->page_table.entries[L1_index].entries == NULL ||
      process->page_table.entries[L1_index].entries[L2_index].valid == 0) {
    return ERROR;
  }

  ppn = process->page_table.entries[L1_index].entries[L2_index].frame;
  main_memory->pages[ppn]->data[address & (PAGE_SIZE - 1)] = *byte;
  write_TLB(process->pid, address >> OFFSET_BITS, ppn);

  return SUCCESS;
}
