// TOO - IF POSSIBLE WRITE  TEST CASES FOR ALL FUNCTIONS
#include <kernel/fos.h>

void
test_multiboot_check(multiboot_info_t* mbd, unsigned int magic)
{
  int test_value;
  int c;
  switch (c) {
    case 0:
      test_value = multiboot_check(mbd, magic);
      if (test_value == -1 || test_value >= 1000)
        return;
      else {
        kprintf("MULTIBOOT CHECK TEST FAILED, with: %d\n", test_value);
        STOP;
      }
  }
}

void
test_everything()
{
  test_multiboot_check(memory_t.mem_db, memory_t.magic);
}