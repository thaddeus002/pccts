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
 *
 * ANTLR 1.33
 * Terence Parr
 * Parr Research Corporation
 * with Purdue University and AHPCRC, University of Minnesota
 * 1989-2001
 */

/**
 * \file hash.c
 * \brief define hash table entries, sizes, hash function...
 *
 * Manage hash tables.
 * A HashTable is a collection of Entry classed by hash of str.
 *
 * The following functions are visible:
 *
 *    Entry **newHashTable();   Create and return initialized hash table
 *    Entry *hash_add(Entry **, char *, Entry *);
 *    Entry *hash_get(Entry **, char *);
 *    void  killHashTable(HashTable table);
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "antlr_log.h"
#include "hash.h"


/** number of entries in the table */
static unsigned int size = HashTableSize;


/**
 * Hash 's' using 'size'.
 * \param s the string to hash
 * \param size max of return value + 1
 */
static unsigned int hash(const char *s, unsigned int size) {
    unsigned int h = 0;

    while ( *s != '\0' ) {
        h = (h<<1) + *s++;
    }
    return (h % size);
}


/** create the hash table and string table for terminals (string table only once) */
HashTable newHashTable()
{
    HashTable table;

    table = calloc(size, sizeof(Entry *));
    require( table != NULL, "cannot allocate hash table");
    return table;
}

void killHashTable(HashTable table)
{
  /* for now, just free table, forget entries */
  free( (char *) table );
}

/**
 * Given a table, add 'rec' with key 'key' (add to front of list).
 * \param table the hash table
 * \param key key to use to retrieve the record (will give the hash)
 * \param rec the record to store
 * \return ptr to entry
 */
Entry *hash_add(HashTable table, char *key, Entry *rec)
{
  unsigned int h=0;
  char *p=key;
  require(table!=NULL && key!=NULL && rec!=NULL, "add: invalid addition");

  h=hash(p,size);
  rec->next = table[h];     /* Add to singly-linked list */
  table[h] = rec;
  return rec;
}

/**
 * Return ptr to 1st entry found in table under key (return NULL if none found)
 * \param table the hashtable
 * \param key finding key
 * \return the last Entry stored with Entry.str == key;
 */
Entry *hash_get(HashTable table, char *key)
{
  unsigned h=0;
  char *p=key;
  Entry *q;
  require(table!=NULL && key!=NULL, "get: invalid table and/or key");

  h=hash(p, size);
  for (q = table[h]; q != NULL; q = q->next)
  {
    if ( !strcmp(key, q->str) ) return( q );
  }
  return( NULL );
}


/**
 * Given a string, this function allocates and returns a pointer to a
 * hash table record of size 'sz' whose "str" pointer is reset to a position
 * in the string table.
 * \param text the entry's text. must not be NULL.
 * \param sz size of the new Entry TODO what this means ?
 */
Entry *newEntry(char *text, int sz)
{
  Entry *p;
  require(text!=NULL, "new: NULL terminal");

  p = (Entry *) calloc(1,sz);

  if ( p == NULL )
  {
    fatal_internal("newEntry: out of memory for terminals\n");
    exit(EXIT_FAILURE);
  }

  p->str = strdup(text);
  // TODO log a warning if p->str==NULL

  return(p);
}


