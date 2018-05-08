/**
 * \file antlr_log.h
 */

#ifndef __ANTLR_LOGGER_H_
#define __ANTLR_LOGGER_H_


          /* E r r o r  M a c r o s */

#define fatal(err)  fatalFL(err, __FILE__, __LINE__)
#define fatal_internal(err) fatal_intern(err, __FILE__, __LINE__)

        /* S a n i t y  C h e c k i n g */

#define require(expr, err) {if ( !(expr) ) fatal_internal(err);}




void warn(char *err);


void err(char *err);


void dlgerror(const char *s);

void fatalFL(char *err_, char *f, int l);

void fatal_intern(char *err_, char *f, int l);

/**
 * Close 'DefFile' if needed.
 */
void cleanUp(void);



#endif
