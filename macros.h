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
#ifndef MACROS_H
#define MACROS_H

#define BIT_SET(a, b)(a |=  (1 << b))	/* a=target variable, b=bit number to act upon 0-n */ 
#define BIT_CLR(a, b)(a &= ~(1 << b))	/* a=target variable, b=bit number to act upon 0-n */ 
#define BIT_FLP(a, b)(a ^=  (1 << b))	/* a=target variable, b=bit number to act upon 0-n */ 
#define BIT_CHK(a, b)(a &   (1 << b))	/* a=target variable, b=bit number to act upon 0-n */
//#define CALC_INDEX(l, c)(c+ ((l >> 3) << 5))
#endif
