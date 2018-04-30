/**
 * \file logger.h
 * Log utilities.
 */

#ifndef __LOGGER_H_
#define __LOGGER_H_



          /* E r r o r  M a c r o s */

#define fatal(err)  fatalFL(err, __FILE__, __LINE__)
#define fatal_internal(err) fatal_intern(err, __FILE__, __LINE__)



        /* S a n i t y  C h e c k i n g */

#define require(expr, err) {if ( !(expr) ) fatal_internal(err);}



/**
 * Print the log header indicating a filename and a line number.
 */
void hdrLog(char *f,int l);

void warn(char *err);

/**
 * Log an error message.
 */
void errorNoFL(char *err, ...);

void err(char *err);

void fatalFL(char *err_, char *f, int l);

void fatal_intern(char *err_, char *f, int l);

/**
 * Log an error message and quit the program.
 */
void fatalNM(char *err, char *file, int line, ...);

/**
 * Log a warning message.
 */
void warningNoFL(char *err, ...);

/**
 * Log a warning message.
 */
void warning(char *err, char *file, int line, ...);

/**
 * Log an error message.
 */
void errorFL(char *err, char *file, int line, ...);

/**
 * Add an info on a new line to stderr.
 * \param err the info to show
 * \param file filename
 * \param line line number the info is for
 */
void log_more(char *err, char *file, int line, ...);


void dlgerror(const char *s);

/**
 * Close 'DefFile' if needed.
 */
void cleanUp(void);

/**
 * Print an error message with variable arg list in a string.
 * \param err format
 * \return a statically allocated string (don't free it). Each call of
 * this function will override previous calls.
 */
char *eMsg(char *err, ...);

#endif
