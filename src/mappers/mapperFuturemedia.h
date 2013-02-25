/*
 * mapperFuturemedia.h
 *
 *  Created on: 28/set/2011
 *      Author: fhorse
 */

#ifndef MAPPERFUTUREMEDIA_H_
#define MAPPERFUTUREMEDIA_H_

#include "common.h"

struct _futuremedia {
	BYTE delay;
} futuremedia;

void map_init_Futuremedia(void);
void extcl_cpu_wr_mem_Futuremedia(WORD address, BYTE value);
BYTE extcl_save_mapper_Futuremedia(BYTE mode, BYTE slot, FILE *fp);
void extcl_cpu_every_cycle_Futuremedia(void);
void extcl_irq_A12_clock_Futuremedia(void);

#endif /* MAPPERFUTUREMEDIA_H_ */
