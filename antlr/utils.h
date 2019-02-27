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
 * \return 1 if the strings are equal, 0 otherwise
 */
int ci_strequ(char *a, char *b);

/**
 * Check if string e is a word in string s.
 * @return 1 for true and 0 for false
 */
int strmember(char *s, char *e);

/**
 * Given "s" yield s (same string without quotes).
 * CAUTION : This function is DESTRUCTIVE (modify s if starts with " else return s)
 * @return the modified string
 */
char *StripQuotes(char *s);

/**
 * remove the quotes from a string if there are some.
 * @param string the string to modify
 * @return the param "string" 
 */
char *strip_quotes(char *string);

/**
 * Allocate memory and copy a string.
 * @param original the string to copy
 * @return a pointer to the newly allocated string or NULL if allocating
 * failled
 */
char *copy_string(const char *original);

#endif
