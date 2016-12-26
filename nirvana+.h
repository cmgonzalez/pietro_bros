/*
	This file is part of Pietro Bros.

	Pietro Bros is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Pietro Bros is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Pietro Bros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef NIRVANAP_H
#define NIRVANAP_H

// This file allows NIRVANAP_TOTAL_ROWS to be consistently defined prior to inclusion of the library header.
// The library header uses this constant to compute the location of Nirvana's ISR hook.

#define NIRVANAP_TOTAL_ROWS 19
#include <arch/zx/nirvana+.h>

#endif
