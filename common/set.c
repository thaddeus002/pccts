/**
    \file set.c

    \brief The following is a general-purpose set library originally developed
    by Hank Dietz and enhanced by Terence Parr to allow dynamic sets.

    Sets are now structs containing the #words in the set and
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

    set
    set_of2(e,g)
    unsigned e,g;
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

    Made it smell less bad to C++ 7/31/93 -- TJP
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"


#define MIN(i,j) ( (i) > (j) ? (j) : (i))
#define MAX(i,j) ( (i) < (j) ? (j) : (i))

/* elems can be a maximum of 32 bits */
static unsigned bitmask[] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000
};

/** empty set */
set empty = set_init;

/** 1 */
static unsigned int min=1;

/** */
#define STR_SIZE     200


/** make arg1 a set big enough to hold max elem # of arg2 */
void set_new(set *a, unsigned int _max) {
  if ((a->setword=calloc(NumWords(_max),BytesPerWord))==NULL) {
    fprintf(stderr, "set_new: Cannot allocate set with max of %u\n", _max);
  }
  a->n = NumWords(_max);
}


/** free memory used by a set */
void set_destroy(set *a) {
  if ( a->setword != NULL ) {
    free(a->setword);
  }
  *a = empty;
}


/**
 * Set the minimum size (in words) of a set to reduce realloc calls.
 */
void set_size(unsigned int n)
{
    min = n;
}

/**
 * Fast compute degree of a set... the number of elements present in the
 * set. Assumes that all word bits are used in the set and that
 * SETSIZE(a) is a multiple of WORDSIZE.
 */
unsigned int set_deg(set a)
{
    register unsigned *p = &(a.setword[0]);
    register unsigned *endp = NULL; /* MR27 Avoid false memory check report */
    register unsigned degree = 0;

    if ( a.n == 0 ) return(0);
    endp = &(a.setword[a.n]);
    while ( p < endp )
    {
        register unsigned t = *p;
        register unsigned *b = &(bitmask[0]);
        do {
            if (t & *b) ++degree;
        } while (++b < &(bitmask[WORDSIZE]));
        p++;
    }

    return(degree);
}


/**
 * Extend (or shrink) the set passed in to have n words.
 *
 * if n is smaller than the minimum, boost n to have the minimum.
 * if the new set size is the same as the old one, do nothing.
 *
 * TJP 4-27-92 Fixed so won't try to alloc 0 bytes
 */
static void set_ext(set *a, unsigned int n)
{
    register unsigned *p;
    register unsigned *endp;
    unsigned int size;

    if ( a->n == 0 )
    {
        if ( n == 0 ) return;
        if (a->setword != NULL) {
            free (a->setword);  /* MR20 */
        }
        a->setword = (unsigned *) calloc(n, BytesPerWord);
        if ( a->setword == NULL )
        {
            fprintf(stderr, "set_ext(%d words): cannot allocate set\n", n);
            exit(-1);
        }
        a->n = n;
        return;
    }
    if ( n < min ) n = min;
    if ( a->n == n || n == 0 ) return;
    size = a->n;
    a->n = n;
    a->setword = (unsigned *) realloc( (char *)a->setword, (n*BytesPerWord) );
    if ( a->setword == NULL )
    {
        fprintf(stderr, "set_ext(%d words): cannot allocate set\n", n);
        exit(-1);
    }

    p    = &(a->setword[size]);     /* clear from old size to new size */
    endp = &(a->setword[a->n]);
    do {
        *p++ = 0;
    } while ( p < endp );
}


/**
 * Fast set union operation
 * resultant set size is max(b, c);
 */
set set_or(set b, set c)
{
    set *big;
    set t;
    unsigned int m,n;
    register unsigned *r, *p, *q, *endp;

    t = empty;
    if (b.n > c.n) {big= &b; m=b.n; n=c.n;} else {big= &c; m=c.n; n=b.n;}
    set_ext(&t, m);
    r = t.setword;

    /* Or b,c until max of smaller set */
    q = c.setword;
    p = b.setword;
    endp = &(b.setword[n]);
    while ( p < endp ) *r++ = *p++ | *q++;

    /* Copy rest of bigger set into result */
    p = &(big->setword[n]);
    endp = &(big->setword[m]);
    while ( p < endp ) *r++ = *p++;

    return(t);
}

/**
 * Fast set intersection operation
 * resultant set size is min(b, c);
 */
set set_and(set b, set c)
{
    set t;
    unsigned int n;
    register unsigned *r, *p, *q, *endp;

    t = empty;
    n = (b.n > c.n) ? c.n : b.n;
    if ( n == 0 ) return t;     /* TJP 4-27-92 fixed for empty set */
    set_ext(&t, n);
    r = t.setword;

    /* & b,c until max of smaller set */
    q = c.setword;
    p = b.setword;
    endp = &(b.setword[n]);
    while ( p < endp ) *r++ = *p++ & *q++;

    return(t);
}

/**
 * Fast set difference operation b - c
 * resultant set size is size(b)
 */
set set_dif(set b, set c)
{
    set t;
    unsigned int n;
    register unsigned *r, *p, *q, *endp;

    t = empty;
    n = (b.n <= c.n) ? b.n : c.n ;
    if ( b.n == 0 ) return t;       /* TJP 4-27-92 fixed for empty set */
                                    /* WEC 12-1-92 fixed for c.n = 0 */
    set_ext(&t, b.n);
    r = t.setword;

    /* Dif b,c until smaller set size */
    q = c.setword;
    p = b.setword;
    endp = &(b.setword[n]);
    while ( p < endp ) *r++ = *p++ & (~ *q++);

    /* Copy rest of b into result if size(b) > c */
    if ( b.n > n )
    {
        p = &(b.setword[n]);
        endp = &(b.setword[b.n]);
        while ( p < endp ) *r++ = *p++;
    }

    return(t);
}

/** Fast singleton set constructor operation */
set set_of(unsigned b)
{
    static set a;

    if ( b == nil ) return( empty );
    set_new(&a, b);
    a.setword[DIVWORD(b)] = bitmask[MODWORD(b)];

    return(a);
}


/**
 * Fast not of set a (assumes all bits used)
 * size of resultant set is size(a)
 * ~empty = empty cause we don't know how bit to make set
 */
set set_not(set a)
{
    set t;
    register unsigned *r;
    register unsigned *p = a.setword;
    register unsigned *endp = &(a.setword[a.n]);

    t = empty;
    if ( a.n == 0 ) return( empty );
    set_ext(&t, a.n);
    r = t.setword;

    do {
        *r++ = (~ *p++);
    } while ( p < endp );

    return(t);
}

/**
 * Check for a equal to b.
 * work with sets of different sizes.
 */
int set_equ(set a, set b)
{
    unsigned int count;
    unsigned int i;

    count=MIN(a.n,b.n);
    if (count == 0) return 1;
    for (i=0; i < count; i++) {
      if (a.setword[i] != b.setword[i]) return 0;
    };
    if (a.n < b.n) {
      for (i=count; i < b.n; i++) {
        if (b.setword[i] != 0) return 0;
      }
      return 1;
    } else if (a.n > b.n) {
      for (i=count; i < a.n; i++) {
        if (a.setword[i] != 0) return 0;
      }
      return 1;
    } else {
      return 1;
    };
}

/**
 * Check for a is a PROPER subset of b.
 */
int set_sub(set a, set b)
{
    unsigned int    count;
    unsigned int    i;

    if (a.n == 0) return 1;
    count=MIN(a.n,b.n);
    for (i=0; i < count; i++) {
      if (a.setword[i] & ~b.setword[i]) return 0;
    };
    if (a.n <= b.n) {
      return 1;
    } else {
      for (i=count; i<a.n ; i++) {
        if (a.setword[i]) return 0;
      };
    };
    return 1;
}


/**
 * Fast pick any element of the set b
 * \return nil if there is no element in b
 */
unsigned set_int(set b)
{
    register unsigned *p = b.setword;
    register unsigned *endp = &(b.setword[b.n]);

    if ( b.n == 0 ) return(nil);

    do {
        if (*p) {
            /* Found a non-empty word of the set */
            register unsigned i = ((p - b.setword) << LogWordSize);
            register unsigned t = *p;
            p = &(bitmask[0]);
            while (!(*p & t)) {
                ++i; ++p;
            }
            return(i);
        }
    } while (++p < endp);

    /* Empty -- only element it contains is nil */
    return(nil);
}

int set_el(unsigned b, set a)
{
    /* nil is an element of every set */
    if (b == nil) return(1);
    if ( a.n == 0 || NumWords(b) > a.n ) return(0);

    /* Otherwise, we have to check */
    return( a.setword[DIVWORD(b)] & bitmask[MODWORD(b)] );
}

/**
 * Fast check for nil set
 * \param a the set to check
 * \return 0 (false) if any word is non-zero in the set a
 */
int set_nil(set a)
{
    unsigned int *p = a.setword;
    unsigned int *endp;

    if ( a.n == 0 ) return(1);
    endp = a.setword + a.n;

    /* The set is not empty if any word used to store
       the set is non-zero.  This means one must be a
       bit careful about doing things like negation.
    */
    do {
        if (*p) return(0);
    } while (++p < endp);

    return(1);
}

/**
 * Fast convert set a into ASCII char string...
 * assumes that all word bits are used in the set
 * and that SETSIZE is a multiple of WORDSIZE.
 * Trailing 0 bits are removed from the string.
 * if no bits are on or set is empty, "" is returned.
 */
char *set_str(set a)
{
    unsigned int *p = a.setword;
    unsigned int *endp = a.setword + a.n;
    static char str_tmp[STR_SIZE+1];
    char *q = str_tmp;

    if (a.n == 0) {
        str_tmp[0]='\0';
        return(str_tmp);
    }

    do {
        unsigned int t = *p;
        unsigned int *b = bitmask;
        do {
            *(q++) = (t & *b) ? '1' : '0';
        } while (++b < bitmask+WORDSIZE);
    } while (++p < endp);

    /* Trim trailing 0s & NULL terminate the string */
    while ((q > str_tmp) && (*(q-1) != '1')) --q;
    *q = '\0';

    return(str_tmp);
}


/**
 * Fast convert set ASCII char string into a set.
 * If the string ends early, the remaining set bits
 * are all made zero.
 * The resulting set size is just big enough to hold all elements.
 */
set set_val(register char *s)
{
    static set a;
    register unsigned *p, *endp;

    set_new(&a, ((unsigned)strlen(s)));
    p = a.setword;
    endp = &(a.setword[a.n]);
    do {
        register unsigned *b = &(bitmask[0]);
        /* Start with a word with no bits on */
        *p = 0;
        do {
            if (*s) {
                if (*s == '1') {
                    /* Turn-on this bit */
                    *p |= *b;
                }
                ++s;
            }
        } while (++b < &(bitmask[WORDSIZE]));
    } while (++p < endp);

    return(a);
}

/**
 * Or element e into set a.  a can be empty.
 */
void set_orel( unsigned e, set *a )
{
    if ( e == nil ) return;
    if ( NumWords(e) > a->n ) set_ext(a, NumWords(e));
    a->setword[DIVWORD(e)] |= bitmask[MODWORD(e)];
}

/**
 * Or set b into set a.  a can be empty. does nothing if b empty.
 */
void set_orin( set *a, set b )
{
    /* Fast set union operation */
    /* size(a) is max(a, b); */
    unsigned int m;
    register unsigned *p,
                      *q    = b.setword,
                      *endq; /* MR20 */

    if ( b.n == 0 ) return;
    endq = &(b.setword[b.n]); /* MR20 */
    m = (a->n > b.n) ? a->n : b.n;
    set_ext(a, m);
    p = a->setword;
    do {
        *p++ |= *q++;
    } while ( q < endq );
}

/**
 * And set b into set a.  a can be empty. does nothing if b empty.
 */
void set_andin( set *a, set b )
{
    /* Fast set intersection operation */
    /* size(a) is max(a, b); */
    unsigned int m;
    register unsigned *p,
                      *q    = b.setword,
                      *endq = &(b.setword[b.n]);

    if ( b.n == 0 ) return;
    m = (a->n > b.n) ? a->n : b.n;
    set_ext(a, m);
    p = a->setword;
    do {
        *p++ &= *q++;
    } while ( q < endq );
}

/**
 * Remove element e from set a.
 * Does not affect size of set
 */
void set_rm(unsigned e, set a)
{
    if ( (e == nil) || (NumWords(e) > a.n) ) return;
    a.setword[DIVWORD(e)] ^= (a.setword[DIVWORD(e)]&bitmask[MODWORD(e)]);
}

/**
 * Remove all elements in a set.
 * Does not effect size of set.
 */
void set_clr(set a)
{
    register unsigned *p = a.setword;
    register unsigned *endp;

    if ( a.n == 0 ) return;
    endp = &(a.setword[a.n]);
    do {
        *p++ = 0;
    } while ( p < endp );
}

set set_dup(set a)
{
    set b;
    register unsigned *p,
                      *q    = a.setword,
                      *endq; /* MR20 */

    b = empty;
    if ( a.n == 0 ) return( empty );
    endq = &(a.setword[a.n]);   /* MR20 */
    set_ext(&b, a.n);
    p = b.setword;
    do {
        *p++ = *q++;
    } while ( q < endq );

    return(b);
}

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
void _set_pdq( set a, register unsigned *q )
{
    register unsigned *p = a.setword,
                      *endp = &(a.setword[a.n]);
    register unsigned e=0;

    /* are there any space (possibility of elements)? */
    if ( a.n == 0 ) return;
    do {
        register unsigned t = *p;
        register unsigned *b = &(bitmask[0]);
        do {
            if ( t & *b ) *q++ = e;
            ++e;
        } while (++b < &(bitmask[WORDSIZE]));
    } while (++p < endp);
    *q = nil;
}

/**
 * Same as _set_pdq except allocate memory.  set_pdq is the natural function
 * to use.
 */
unsigned *set_pdq(set a)
{
    unsigned *q;
    int max_deg;

    max_deg = WORDSIZE*a.n;
    /* assume a.n!=0 & no elements is rare, but still ok */
    if ( a.n == 0 ) return(NULL);
    q = (unsigned *) malloc((max_deg+1)*BytesPerWord);
    if ( q == NULL ) return( NULL );
    _set_pdq(a, q);
    return( q );
}

/**
 * A function that produces a hash number for the set.
 */
unsigned int set_hash(set a, register unsigned int mod)
{
    /* Fast hash of set a (assumes all bits used) */
    register unsigned *p = &(a.setword[0]);
    register unsigned *endp = &(a.setword[a.n]);
    register unsigned i = 0;

    while (p<endp){
        i += (*p);
        ++p;
    }

    return(i % mod);
}
