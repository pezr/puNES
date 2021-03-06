/*
 *  Copyright (C) 2010-2016 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef MAPPER_VRC6_H_
#define MAPPER_VRC6_H_

#include "common.h"

enum { VRC6A, VRC6B };

typedef struct {
	BYTE enabled;
	BYTE duty;
	BYTE step;
	BYTE volume;
	BYTE mode;
	WORD timer;
	WORD frequency;
	SWORD output;

/* ------------------------------------------------------- */
/* questi valori non e' necessario salvarli nei savestates */
/* ------------------------------------------------------- */
/* */ BYTE clocked;                                     /* */
/* ------------------------------------------------------- */
} _vrc6_square;
typedef struct {
	BYTE enabled;
	BYTE accumulator;
	BYTE step;
	BYTE internal;
	WORD timer;
	WORD frequency;
	SWORD output;

/* ------------------------------------------------------- */
/* questi valori non e' necessario salvarli nei savestates */
/* ------------------------------------------------------- */
/* */ BYTE clocked;                                     /* */
/* ------------------------------------------------------- */
} _vrc6_saw;
struct _vrc6 {
	BYTE enabled;
	BYTE reload;
	BYTE mode;
	BYTE acknowledge;
	BYTE count;
	BYTE delay;
	WORD prescaler;
	_vrc6_square S3, S4;
	_vrc6_saw saw;
} vrc6;

void map_init_VRC6(BYTE revision);
void extcl_cpu_wr_mem_VRC6(WORD address, BYTE value);
BYTE extcl_save_mapper_VRC6(BYTE mode, BYTE slot, FILE *fp);
void extcl_cpu_every_cycle_VRC6(void);
void extcl_apu_tick_VRC6(void);

#endif /* MAPPER_VRC6_H_ */
