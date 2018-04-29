#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"

// define ErrHdr "%s, line %d:"
#include "constants.h"
// MaxNumFiles
#include "generic.h"
// for zzline
#include "dlgdef.h"

// from globals.c and declared in proto.h
extern FILE *DefFile;
extern int CurFile;
extern char *FileStr[MaxNumFiles];/* Ptr to array of file names on command-line */
extern int NumFiles;     /* current grammar file number */


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

void warnNoFL(char *err)
{
    warning(err, NULL, 0);
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
 * Log a warning message.
 */
void warning(char *err, char *file, int line, ...) {
    va_list ap;
    va_start(ap, line);
    log_message_va(WARNING, err, file, line, ap);
    va_end(ap);
}

void warn(char *err)
{
    /* back up the file number if we hit an error at the end of the last file */
    if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
    hdrLog(FileStr[CurFile], zzline);
    fprintf(stderr, " warning: %s\n", err);
}

void errNoFL(char *err)
{
    fprintf(stderr, " error: %s\n", err);
}

void errFL(char *err,char *f,int l)
{
    hdrLog(f, l);
    fprintf(stderr, " error: %s\n", err);
}

void err(char *err)
{
    /* back up the file number if we hit an error at the end of the last file */
    if ( CurFile >= NumFiles && CurFile >= 1 ) CurFile--;
    hdrLog(FileStr[CurFile], zzline);
    fprintf(stderr, " error: %s\n", err);
}


void fatalFL(char *err, char *f, int l )
{
    hdrLog(f, l);
    fprintf(stderr, " %s\n", err);
    cleanUp();
    exit(EXIT_FAILURE);
}


/** TODO : replace fatalFL or remove this function */
void fatale(char *err, char *file, int line) {
    fprintf(stderr, "%s(%d): %s\n", __FILE__, line, err);
    exit(EXIT_FAILURE);
}

/** TODO : replace fatalFL or remove this function */
void fatal_intern(char *err_, char *f, int l)
{
    hdrLog(f, l);
    fprintf(stderr, " #$%%*&@# internal error: %s\n", err_);
    hdrLog(f, l);
    fprintf(stderr, " [complain to nearest government official\n");
    hdrLog(f, l);
    fprintf(stderr, "  or send hate-mail to parrt@parr-research.com;\n");
    hdrLog(f, l);
    fprintf(stderr, "  please pray to the ``bug'' gods that there is a trival fix.]\n");
    cleanUp();
    exit(EXIT_FAILURE);
}

/**
 * Close 'DefFile' if needed.
 */
void cleanUp(void) {
    if ( DefFile != NULL) fclose( DefFile );
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



void dlgerror(const char *s)
{
    hdrLog(FileStr[CurFile], zzline);
    fprintf(stderr, " lexical error: %s (text was '%s')\n",
            ((s == NULL) ? "Lexical error" : s), zzlextext);
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
