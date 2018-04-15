/**
 * \file logger.h
 * Log utilities.
 */

#ifndef __LOGGER_H_
#define __LOGGER_H_



void warnNoFL(char *err);

void warnFL(char *err,char *f,int l);

void warn(char *err);

void warnNoCR( char *err );

void errNoFL(char *err);

void errFL(char *err,char *f,int l);

void err(char *err);

void errNoCR( char *err );

void fatalFL( char *err_, char *f, int l );




#endif
