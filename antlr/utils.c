/*
 * Thaddeus002's pccts
 *
 * Public Domain.
 */


/**
 * \file utils.c
 * \brief Utilitaries functions for strings handling.
 */

#include <ctype.h> // toupper(), isalnum()
#include <string.h> // strlen()
#include "antlr_log.h" // requires()
#include "utils.h"


/**
 * Compare two strings case insentive way.
 * \return true if the strings are equal
 */
int ci_strequ(char *a, char *b)
{
    for ( ;*a != '\0' && *b != '\0'; a++, b++) {
        if (toupper(*a) != toupper(*b)) return 0;
    }
    return (*a == *b);
}


/**
 * Check if string e is a word in string s.
 */
int strmember(char *s, char *e)
{
    register char *p;
    require(s != NULL && e != NULL, "strmember: NULL string");

    if (*e == '\0') return 1; /* empty string is always member */

    do {
        while (*s!='\0' && !isalnum(*s) && *s!='_')
        ++s;
        p = e;
        while ( *p!='\0' && *p==*s ) { p++; s++; }
        if ( *p=='\0' ) {
            if ( *s=='\0' ) return 1;
            if ( !isalnum (*s) && *s != '_' ) return 1;
        }
        while ( isalnum(*s) || *s == '_' ) ++s;
    } while (*s != '\0');
    return 0;
}


/**
 * Given "s" yield s (same string without quotes).
 * CAUTION : This function is DESTRUCTIVE (modify s if starts with " else return s)
 * @return the modified string
 */
char *StripQuotes(char *s)
{
    if (*s == '"') {
        s[ strlen(s)-1 ] = '\0'; /* remove last quote */
        return s + 1; /* return address past initial quote */
    }
    return s;
}
