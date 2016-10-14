#ifndef NIRVANAP_H
#define NIRVANAP_H

// This file allows NIRVANAP_TOTAL_ROWS to be consistently defined prior to inclusion of the library header.
// The library header uses this constant to compute the location of Nirvana's ISR hook.

#define NIRVANAP_TOTAL_ROWS 19
#include <arch/zx/nirvana+.h>

#endif
