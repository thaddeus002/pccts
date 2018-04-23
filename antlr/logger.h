/**
 * \file logger.h
 * Log utilities.
 */

#ifndef __LOGGER_H_
#define __LOGGER_H_


/**
 * Print the log header indicating a filename and a line number.
 */
void hdrLog(char *f,int l);

void warnNoFL(char *err);

void warnFL(char *err,char *f,int l);

void warn(char *err);

void errNoFL(char *err);

void errFL(char *err,char *f,int l);

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
void warning(char *err, char *file, int line, ...);

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


#endif
