// DEFINE OWN MEMORY MAP

#pragma output __MMAP                = -1         // memory map is provided in file "mmap.inc"

// COMPILE TIME CRT CONFIGURATION

#pragma output CRT_ORG_CODE          = 23552      // org of compile
#pragma output REGISTER_SP           = -1         // crt will not change stack location; loader sets sp

#pragma output CRT_ENABLE_CLOSE      = 0          // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI       = 0          // don't di on start or exit; loader does di before start
#pragma output CRT_ON_EXIT           = 0          // jump to address 0 at exit

#pragma output CLIB_MALLOC_HEAP_SIZE = 0          // no malloc heap
#pragma output CLIB_STDIO_HEAP_SIZE  = 0          // no stdio heap (no files)

#pragma output CLIB_FOPEN_MAX        = -1         // no FILE* list
