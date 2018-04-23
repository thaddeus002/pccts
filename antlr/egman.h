/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */

/**
 * \file egman.h
 */

#ifndef __EGMAN_H__
#define __EGMAN_H__

#include "syn.h"
#include "generic.h"

/**
 * Called to add an exception group for an alternative EG.
 */
void egAdd(ExceptionGroup * eg);


void leAdd(LabelEntry * le);


void altAdd(Junction *alt);


/** always call leFixup() BEFORE egFixup() */
void egFixup();


void leFixup();


/** always call altFixup() BEFORE egFixup() */
void altFixup();

#endif
