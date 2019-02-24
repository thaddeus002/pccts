/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */


/**
 * \file utils.h
 * \brief Utilitaries functions for strings handling.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * Compare two strings case insentive way.
 * \return true if the strings are equal
 */
int ci_strequ(char *a, char *b);

/**
 * Check if string e is a word in string s.
 */
int strmember(char *s, char *e);

/**
 * Given "s" yield s (same string without quotes).
 * CAUTION : This function is DESTRUCTIVE (modify s if starts with " else return s)
 * @return the modified string
 */
char *StripQuotes(char *s);

#endif
