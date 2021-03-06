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
#include <string.h> // strlen(), strcpy()
#include <stdlib.h> // malloc()
#include <stdio.h>
#include "utils.h"


/**
 * Compare two strings case insentive way.
 * \return 1 if the strings are equal, 0 otherwise
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
 * @return 1 for true and 0 for false
 */
int strmember(char *s, char *e)
{
    if(s==NULL || e==NULL) {
        fprintf(stderr, "strmember: NULL string\n");
        exit(1);
    }

    if (*e == '\0') return 1; /* empty string is always member */

    do {
        char *p;
        while (*s!='\0' && !isalnum(*s) && *s!='_') ++s;
        p = e;
        while ( *p!='\0' && *p==*s ) { p++; s++; }
        if ( *p == '\0' ) {
            if ( *s=='\0' ) return 1;
            if ( !isalnum (*s) && *s != '_' ) return 1;
        }
        while (isalnum(*s) || *s == '_' ) ++s;
    } while (*s != '\0');
    return 0;
}


/**
 * remove the quotes from a string if there are some.
 * @param string the string to modify
 * @return the param "string" 
 */
char *strip_quotes(char *string)
{
    if (*string == '"' && string[strlen(string)-1]=='"') {
        int i = 0;
        while(string[i]!='\0') {
            string[i] = string[i+1];
            i++;
        }
        string[strlen(string)-1]='\0';
    }
    return string;
}


/**
 * Allocate memory and copy a string.
 * @param original the string to copy
 * @return a pointer to the newly allocated string or NULL if allocating
 * failled
 */
char *copy_string(const char *original) {

    char *copy;

    if(original == NULL) {
        return NULL;
    }
    
    copy = malloc(sizeof(char) * (strlen(original) + 1));
    if (copy != NULL) {
        strcpy(copy, original);
    }

    return copy;
}
