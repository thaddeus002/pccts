/**
    \file set.h

    \brief The following is a general-purpose set library originally developed
    by Hank Dietz and enhanced by Terence Parr to allow dynamic sets.

    Sets are now structs containing the number of words in the set and
    a pointer to the actual set words.

    Generally, sets need not be explicitly allocated.  They are
    created/extended/shrunk when appropriate (e.g. in set_of()).
    HOWEVER, sets need to be destroyed (free()ed) when they go out of scope
    or are otherwise no longer needed.  A routine is provided to
    free a set.

    Sets can be explicitly created with set_new(s, max_elem).

    Sets can be declared to have minimum size to reduce realloc traffic.
    Default minimum size = 1.

    Sets can be explicitly initialized to have no elements (set.n == 0)
    by using the 'empty' initializer:

    Examples:
        set a = empty;  -- set_deg(a) == 0

        return( empty );

    Example set creation and destruction:

    set set_of2(unsigned e, unsigned g)
    {
        set a,b,c;

        b = set_of(e);      -- Creates space for b and sticks in e
        set_new(c, g);      -- set_new(); set_orel() ==> set_of()
        set_orel(g, &c);
        a = set_or(b, c);
        .
        .
        .
        set_free(b);
        set_free(c);
        return( a );
    }

    1987 by Hank Dietz

    Modified by:
        Terence Parr
        Purdue University
        October 1989

*/

#ifndef __GATE_SET_H
#define __GATE_SET_H


/* Define usable bits per unsigned int word */
#define BytesPerWord    sizeof(unsigned)
#define WORDSIZE        (sizeof(unsigned)*8)
#define LogWordSize     (WORDSIZE==16?4:5)

#define SETSIZE(a) ((a).n<<LogWordSize)     /* Maximum items per set */
#define MODWORD(x) ((x) & (WORDSIZE-1))     /* x % WORDSIZE */
#define DIVWORD(x) ((x) >> LogWordSize)     /* x / WORDSIZE */
#define nil (~((unsigned) 0))   /* An impossible set member all bits on (big!) */

/**
 * The set struct, containing various words.
 */
typedef struct _set {
    unsigned int n; /**< Number of words in set */
    unsigned int *setword; /**< content */
} set;


/* M a c r o s */

/** An empty set */
#define set_init    {0, NULL}

/** Verify that a set is null */
#define set_null(a) ((a).setword==NULL)

/** Num words to hold x */
#define NumWords(x)     ((((unsigned)(x))>>LogWordSize)+1)

/** Global variable for an empty set */
extern set empty;


#define set_free(a)                                 \
    {if ( (a).setword != NULL ) free((char *)((a).setword));    \
    (a) = empty;}

/** free memory used by a set */
void set_destroy(set *a);


/**
 * Set the minimum size (in words) of a set to reduce realloc calls.
 * This is global config function.
 */
void set_size(unsigned int n);

/**
 * Fast compute degree of a set... the number of elements present in the
 * set. Assumes that all word bits are used in the set and that
 * SETSIZE(a) is a multiple of WORDSIZE.
 */
unsigned int set_deg(set a);

/**
 * Fast set union operation
 * resultant set size is max(b, c);
 */
set set_or(set b, set c);

/**
 * Fast set intersection operation
 * resultant set size is min(b, c);
 */
set set_and(set b, set c);

/**
 * Fast set difference operation b - c
 * resultant set size is size(b)
 */
set set_dif(set, set);

/** Fast singleton set constructor operation */
set set_of(unsigned b);

/**
 * Extend (or shrink) the set passed in to have n words.
 *
 * if n is smaller than the minimum, boost n to have the minimum.
 * if the new set size is the same as the old one, do nothing.
 *
 * TJP 4-27-92 Fixed so won't try to alloc 0 bytes
 */
void set_ext(set *a, unsigned int n);

/**
 * Fast not of set a (assumes all bits used)
 * size of resultant set is size(a)
 * ~empty = empty cause we don't know how bit to make set
 */
set set_not(set a);

/**
 * Check for a equal to b.
 * work with sets of different sizes.
 */
int set_equ(set a, set b);

/**
 * Check for a is a PROPER subset of b.
 */
int set_sub(set a, set b);

/**
 * Fast pick any element of the set b
 * \return nil if there is no element in b
 */
unsigned set_int(set b);


int set_el( unsigned, set );

/**
 * Fast check for nil set
 * \param a the set to check
 * \return 0 (false) if any word is non-zero in the set a
 */
int set_nil(set a);

/**
 * Fast convert set a into ASCII char string...
 * assumes that all word bits are used in the set
 * and that SETSIZE is a multiple of WORDSIZE.
 * Trailing 0 bits are removed from the string.
 * if no bits are on or set is empty, "" is returned.
 */
char *set_str(set a);

/**
 * Fast convert set ASCII char string into a set.
 * If the string ends early, the remaining set bits
 * are all made zero.
 * The resulting set size is just big enough to hold all elements.
 */
set set_val(register char *s);

/**
 * Or element e into set a.  a can be empty.
 */
void set_orel(unsigned e, set *a);

/**
 * Or set b into set a.  a can be empty. does nothing if b empty.
 */
void set_orin(set *a, set b);

/**
 * And set b into set a.  a can be empty. does nothing if b empty.
 */
void set_andin(set *a, set b);

/**
 * Remove element e from set a.
 * Does not affect size of set
 */
void set_rm(unsigned, set );

/**
 * Remove all elements in a set.
 * Does not effect size of set.
 */
void set_clr(set a);

set set_dup(set a);

void set_PDQ(set a, register unsigned *q);

/**
 * Same as _set_pdq except allocate memory.  set_pdq is the natural function
 * to use.
 */
unsigned *set_pdq(set a);

/**
 * Return a nil terminated list of unsigned ints that represents all
 * "on" bits in the bit set.
 *
 * e.g. {011011} --> {1, 2, 4, 5, nil}
 *
 * _set_pdq and set_pdq are useful when an operation is required on each element
 * of a set.  Normally, the sequence is:
 *
 *      while ( set_deg(a) > 0 ) {
 *          e = set_int(a);
 *          set_rm(e, a);
 *          ...process e...
 *      }
 * Now,
 *
 *      t = e = set_pdq(a);
 *      while ( *e != nil ) {
 *          ...process *e...
 *          e++;
 *      }
 *      free( t );
 *
 * We have saved many set calls and have not destroyed set a.
 */
void _set_pdq(set a, register unsigned *q);

/**
 * A function that produces a hash number for the set.
 */
unsigned int set_hash(set a, register unsigned int mod);

#endif
