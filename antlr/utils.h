/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */


/**
 * \file utils.h
* \brief Utilitaries functions.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * Compare two string case insentive way.
 * \return true if the strings are equal
 */
int ci_strequ(char *a, char *b);

/** check to see if string e is a word in string s */
int strmember(char *s, char *e);

/** given "s" yield s -- DESTRUCTIVE (we modify s if starts with " else return s) */
char *StripQuotes(char *s);


#endif
