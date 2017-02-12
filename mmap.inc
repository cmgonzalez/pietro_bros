;; PIETRO BROS CUSTOM MEMORY MAP
;;
;; Nirvana+ has been assembled to use 19 rows instead of the
;; usual 23 and this leaves a hole of more than 1300 bytes
;; inside the nirvana+ binary that can be used for other
;; things.
;;
;; To make use of that memory, section NIRVANA_HOLE is
;; created into which some of the program's code and
;; variables are placed.  The placement requires the
;; definition of a custom memory map.
;;
;; To specify a custom memory map to z88dk, the crt variable
;; "__MMAP" must be set to -1 and this is done in file
;; "zpragma.inc".  Then z88dk will look for a file "mmap.inc"
;; to include as the memory map.
;;
;; This memory map is a simple modification of the default
;; memory map defined by z88dk in:
;; 
;; z88dk/libsrc/_DEVELOPMENT/target/crt_memory_model.inc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; memory model ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; sections defined by pietro bros
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION MCLOAD
org 16384

SECTION LOADER
org 23296

;; place zx7 into the loader binary and first thing in memory
;; so that it's available throughout the load process and
;; from the 128k's extra memory banks

section code_compress_zx7
section LOADER_CODE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION BANK_06
org 0xc000

;; 128k only, mainly ay audio

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION NIRVANA_HOLE
org 56718+(328*19)

defb 0    ;; control the name of the output file

;; ensure beeper code is not in contended memory

section code_sound_bit

;; data and bss sections will be placed in hole

SECTION DATA

section smc_driver
section smc_font
section smc_clib
  include "../../clib_smc.inc"
section smc_lib
section smc_compiler
section smc_user

section data_driver
section data_font
section data_clib
  include "../../clib_data.inc"
  ;;section data_fcntl_stdio_heap_head
  ;;section data_fcntl_stdio_heap_body
  ;;section data_fcntl_stdio_heap_tail
  ;;section data_fcntl_fdtable_body
section data_lib
section data_compiler
section data_user

SECTION DATA_END

SECTION BSS

section BSS_UNINITIALIZED

section bss_driver
section bss_font
section bss_clib
  include "../../clib_bss.inc"
section bss_lib
section bss_compiler
section bss_user

SECTION BSS_END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; main code in 48k
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION CODE
org __crt_org_code

section code_crt_init
section code_crt_main
section code_crt_exit
section code_crt_return
section code_crt_common

SECTION FONT

section code_driver
section code_font
section code_clib
  include "../../clib_code.inc"
section code_lib
section code_compiler
section code_user

section rodata_driver
section rodata_font
section rodata_clib
  include "../../clib_rodata.inc"
  ;;section rodata_error_strings
  ;;section rodata_error_string_end
  ;;defb 0
section rodata_lib
section rodata_compiler
section rodata_user

SECTION CODE_END

;; end memory model ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
