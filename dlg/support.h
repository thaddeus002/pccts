/*
 * SOFTWARE RIGHTS
 *
 * We reserve no LEGAL rights to the Purdue Compiler Construction Tool
 * Set (PCCTS) -- PCCTS is in the public domain.  An individual or
 * company may do whatever they wish with source code distributed with
 * PCCTS or the code generated by PCCTS, including the incorporation of
 * PCCTS, or its output, into commerical software.
 *
 * We encourage users to develop software with PCCTS.  However, we do ask
 * that credit is given to us for developing PCCTS.  By "credit",
 * we mean that if you incorporate our source code into one of your
 * programs (commercial product, research project, or otherwise) that you
 * acknowledge this fact somewhere in the documentation, research report,
 * etc...  If you like PCCTS and have developed a nice tool with the
 * output, please mention that you developed it using PCCTS.  In
 * addition, we ask that this header remain intact in our source code.
 * As long as these guidelines are kept, we expect to continue enhancing
 * this system and expect to make other tools available as they are
 * completed.
 */

#ifndef SUPPORT_H__
#define SUPPORT_H__

#include <stdio.h>

/**
 * Allocate 'bytes' bytes, or quit if fail.
 * \param bytes the number of bytes to allocate
 * \param file name of source file
 * \param line line number in which the function is called
 */
char *dlg_malloc(int bytes,char *file,int line);

/**
 * Allocate 'n' times 'bytes' bytes, or quit if fail.
 * \param bytes the number of bytes to allocate
 * \param file name of source file
 * \param line line number in which the function is called
 */
char *dlg_calloc(int n,int bytes,char *file,int line);

/**
 * Open an input stream.
 * \param name file to read
 * \return the new filestream (must be closed), or NULL if fail, or stdin if 'name' is NULL.
 */
FILE *read_stream(char *name);

/**
 * Opens file for writing.
 * \param outputDirectory
 * \param name filename to write
 * \return newly opened stream (that must be closed), or NULL if failed, or stdout if 'name' is NULL
 */
FILE *write_stream(char *outputDirectory, char *name);


void error(char *message,int line_no);

#endif
