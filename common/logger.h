/**
 * \file logger.h
 * Log utilities.
 */

#ifndef __LOGGER_H_
#define __LOGGER_H_



          /* E r r o r  M a c r o s */

#define internal_error(err) internal_error_FL(err, __FILE__, __LINE__)



/**
 * Print the log header indicating a filename and a line number.
 */
void hdrLog(char *f,int l);

/**
 * Log an info message.
 */
void logNoFL(char *err, ...);

/**
 * Log an info message with filename and line number.
 */
void logFL(char *err, char *file, int line, ...);

/**
 * Log an error message.
 */
void errorNoFL(char *err, ...);


/**
 * Log an error message and quit the program.
 */
void fatalNM(char *err, char *file, int line, ...);

/**
 * Quit program if an error occured.
 */
void internal_error_FL(char *err, char *file, int line);

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



/**
 * Print an error message with variable arg list in a string.
 * \param err format
 * \return a statically allocated string (don't free it). Each call of
 * this function will override previous calls.
 */
char *eMsg(char *err, ...);

#endif
