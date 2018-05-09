#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"

#define ErrHdr "%s, line %d:"


/*void require(int expr, char *err) {
    if ( !(expr) ) fatal_intern(err, NULL, 0);
}*/


typedef enum {
    WARNING,
    ERROR,
    FATAL,
    NONE,
    LAST
} LogLevel;



static void log_message(LogLevel level, char *err, char *f, int l) {

    if((f!=NULL) && (l!=0)) {
        hdrLog(f, l);
        fprintf(stderr, " ");
    }

    char *levelName = NULL;

    switch(level) {
      case WARNING:
          levelName = "WARNING:";
          break;
      case ERROR:
          levelName = "ERROR:";
          break;
      case FATAL:
          levelName = "FATAL:";
          break;
      case LAST:
          levelName = " \t\t ";
          break;
      default:
          levelName = ":";
    }
    fprintf(stderr, "%s %s\n", levelName, err);
}


/**
 * Don't use args after this, just va_end() it.
 */
static void log_message_va(LogLevel level, const char *message, char *file, int line, va_list args) {

    va_list args_cpy;
    size_t len = 0;
    char *output = NULL;

    va_copy(args_cpy, args);

    len = vsnprintf(NULL, 0, message, args);
    output = malloc(sizeof(char) * (len+1));
    if(output != NULL) {
        vsnprintf(output, len+1, message, args);
        log_message(level, output, file, line);
        free(output);
    }

    va_end(args_cpy);
}


void hdrLog(char *f,int l) {
    fprintf(stderr, ErrHdr, f, l);
}


/**
 * Log a warning message.
 */
void warningNoFL(char *err, ...) {
    va_list ap;
    va_start(ap, err);
    log_message_va(WARNING, err, NULL, 0, ap);
    va_end(ap);
}

/**
 * Log an info message.
 */
void logNoFL(char *err, ...) {
    va_list ap;
    va_start(ap, err);
    log_message_va(NONE, err, NULL, 0, ap);
    va_end(ap);
}

/**
 * Log an info message with filename and line number.
 */
void logFL(char *err, char *file, int line, ...) {
    va_list ap;
    va_start(ap, line);
    log_message_va(NONE, err, file, line, ap);
    va_end(ap);
}



/**
 * Log a warning message.
 */
void warning(char *err, char *file, int line, ...) {
    va_list ap;
    va_start(ap, line);
    log_message_va(WARNING, err, file, line, ap);
    va_end(ap);
}


/**
 * Log an error message.
 */
void errorNoFL(char *err, ...) {
    va_list ap;
    va_start(ap, err);
    log_message_va(ERROR, err, NULL, 0, ap);
    va_end(ap);
}

/**
 * Log an error message.
 */
void errorFL(char *err, char *file, int line, ...) {
    va_list ap;
    va_start(ap, line);
    log_message_va(ERROR, err, file, line, ap);
    va_end(ap);
}



/**
 * Quit program if an error occured.
 */
void internal_error_FL(char *err, char *file, int line) {
    hdrLog(file, line);
    fprintf(stderr, " FATAL: %s\n", err);
    exit(EXIT_FAILURE);
}


/** TODO : replace fatalFL or remove this function */
void fatale(char *err, char *file, int line) {
    fprintf(stderr, "%s(%d): %s\n", __FILE__, line, err);
    exit(EXIT_FAILURE);
}


/**
 * Log an error message and quit the program.
 */
void fatalNM(char *err, char *file, int line, ...) {
    va_list ap;
    va_start(ap, line);
    log_message_va(FATAL, err, file, line, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}


/**
 * Add an info on a new line to stderr.
 * \param err the info to show
 * \param file filename
 * \param line line number the info is for
 */
void log_more(char *err, char *file, int line, ...) {
    va_list ap;
    va_start(ap, line);
    log_message_va(LAST, err, file, line, ap);
    va_end(ap);
}


/**
 * Print an error message with variable arg list in a string.
 * \param err format
 * \return a statically allocated string (don't free it). Each call of
 * this function will override previous calls.
 */
char *eMsg(char *err, ...) {
    va_list ap;
    static char buf[250]; /* DANGEROUS as hell !!!!!! */

    va_start(ap, err);
    vsprintf(buf, err, ap);
    va_end(ap);
    return(buf);
}
