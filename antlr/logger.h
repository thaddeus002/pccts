/**
 * \file logger.h
 * Log utilities.
 */

#ifndef __LOGGER_H_
#define __LOGGER_H_


          /* E r r o r  M a c r o s */

#define fatal(err)  fatalFL(err, __FILE__, __LINE__)
#define fatal_internal(err) fatal_intern(err, __FILE__, __LINE__)


#define eMsg1(s,a)  eMsg3(s,a,NULL,NULL)
#define eMsg2(s,a,b)  eMsg3(s,a,b,NULL)

        /* S a n i t y  C h e c k i n g */

#ifndef require
#define require(expr, err) {if ( !(expr) ) fatal_internal(err);}
#endif



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

/**
 * Sprintf up to 3 strings.
 * \param s format
 * \param a1 first string
 * \param a2 second string
 * \param a3 third string
 * \return a statically allocated string (don't free it). Each call of
 * this function will override previous calls.
 */
char *eMsg3(char *s, char *a1, char *a2, char *a3);

/** sprintf a decimal */
char *eMsgd(char *s, int d);

char *eMsgd2(char *s, int d1,int d2);


#endif
