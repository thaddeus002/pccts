
/* parser.dlg -- DLG Description of scanner
 *
 * Generated from: dlg_p.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#include "constants.h"
#include <stdio.h>

#include <ctype.h>
#include "dlg_p.h"
#include "output.h"
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "support.h"

LOOKAHEAD

void zzerraction()
{
  (*zzerr)("invalid token");
  zzadvance();
  zzskip();
}

/*
 * D L G tables
 *
 * Generated from: parser.dlg
 *
 * 1989-2001 by  Will Cohen, Terence Parr, and Hank Dietz
 * Purdue University Electrical Engineering
 * DLG Version 1.33MR33
 */

#include "mode.h"




int func_action;    /* should actions be turned into functions?*/
int lex_mode_counter = 0; /* keeps track of the number of %%names */
/* MR1                      */
/* MR1  11-Apr-97 Provide mechanism for inserting code into DLG class */
/* MR1        via <<%%lexmember...>>          */
/* MR1                      */
int lexMember = 0;    /* <<%%lexmemeber ...>>       MR1 */
int lexAction = 0;    /* <<%%lexaction ...>>      MR1 */
int parserClass = 0;  /* <<%%parserclass ...>>        MR1 */
int lexPrefix = 0;    /* <<%%lexprefix ...>>      MR1 */
char  theClassName[100];
char  *pClassName=theClassName;
int firstLexMember=1;


void  xxputc(int c) {
    if (parserClass) {
      *pClassName++=c;
      *pClassName=0;
    } else if (lexMember || lexPrefix) {
      if (class_stream != NULL) fputc(c,class_stream);
    } else {
      fputc(c,output_stream);
    };
  }

void xxprintf(char *format,char *string) {
    if (lexMember || lexPrefix || parserClass) {
        if (class_stream != NULL)
            fprintf(class_stream,format,string);
    } else {
        fprintf(output_stream,format,string);
    };
}

static void act1()
{
    NLA = 1;
}


static void act2()
{
    NLA = 2;
    zzskip();
}


static void act3()
{
    NLA = 3;
    zzline++; zzskip();
}


static void act4()
{
    NLA = L_EOF;
}


static void act5()
{
    NLA = PER_PER;
}


static void act6()
{
    NLA = NAME_PER_PER;
    p_mode_def(&zzlextext[2],lex_mode_counter++);
}


static void act7()
{
    NLA = LEXMEMBER;
    lexMember=1;
    if (firstLexMember != 0) {
      firstLexMember=0;
      p_class_def1();
    };
    zzmode(ACT);
}


static void act8()
{
    NLA = LEXACTION;
    lexAction=1;zzmode(ACT);
}


static void act9()
{
    NLA = PARSERCLASS;
    parserClass=1;
    zzmode(ACT);
}


static void act10()
{
    NLA = LEXPREFIX;
    lexPrefix=1;zzmode(ACT);
}


static void act11()
{
    NLA = ACTION;
    if (func_action)
    fprintf(output_stream,"\n%s %sact%d()\n{ ",
    gen_cpp?"ANTLRTokenType":"static void",
    gen_cpp?ClassName("::"):"", ++action_no);
    zzmode(ACT); zzskip();
}


static void act12()
{
    NLA = GREAT_GREAT;
}


static void act13()
{
    NLA = L_BRACE;
}


static void act14()
{
    NLA = R_BRACE;
}


static void act15()
{
    NLA = L_PAR;
}


static void act16()
{
    NLA = R_PAR;
}


static void act17()
{
    NLA = L_BRACK;
}


static void act18()
{
    NLA = R_BRACK;
}


static void act19()
{
    NLA = ZERO_MORE;
}


static void act20()
{
    NLA = ONE_MORE;
}


static void act21()
{
    NLA = OR;
}


static void act22()
{
    NLA = RANGE;
}


static void act23()
{
    NLA = NOT;
}


static void act24()
{
    NLA = OCTAL_VALUE;
    {int t; sscanf(&zzlextext[1],"%o",&t); zzlextext[0] = t;}
}


static void act25()
{
    NLA = HEX_VALUE;
    {int t; sscanf(&zzlextext[3],"%x",&t); zzlextext[0] = t;}
}


static void act26()
{
    NLA = DEC_VALUE;
    {int t; sscanf(&zzlextext[1],"%d",&t); zzlextext[0] = t;}
  }


static void act27()
{
    NLA = TAB;
    zzlextext[0] = '\t';
  }


static void act28()
{
    NLA = NL;
    zzlextext[0] = '\n';
  }


static void act29()
{
    NLA = CR;
    zzlextext[0] = '\r';
  }


static void act30()
{
    NLA = BS;
    zzlextext[0] = '\b';
  }


static void act31()
{
    NLA = CONTINUATION;
    zzline++; zzskip();
  }


static void act32()
{
    NLA = LIT;
    zzlextext[0] = zzlextext[1];
}


static void act33()
{
    NLA = REGCHAR;
}

static unsigned char shift0[257] = {
  0, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  1, 2, 40, 40, 1, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 1, 40, 40, 40, 40, 4, 40,
  40, 30, 31, 34, 35, 40, 37, 40, 40, 23,
  24, 24, 24, 24, 24, 24, 24, 25, 25, 40,
  40, 26, 40, 27, 40, 3, 21, 21, 21, 21,
  21, 21, 22, 22, 22, 22, 22, 22, 22, 22,
  22, 22, 22, 22, 22, 22, 22, 22, 22, 20,
  22, 22, 32, 39, 33, 40, 22, 40, 11, 9,
  12, 21, 6, 19, 22, 22, 14, 22, 22, 5,
  8, 16, 15, 17, 22, 10, 18, 13, 22, 22,
  22, 7, 22, 22, 28, 36, 29, 38, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
  40, 40, 40, 40, 40, 40, 40
};


static void act34()
{
    NLA = 1;
    error("unterminated action", zzline); zzmode(START);
}


static void act35()
{
    NLA = ACTION;
    if (func_action) fprintf(output_stream,"}\n\n");
    zzmode(START);
    /* MR1                      */
    /* MR1  11-Apr-97 Provide mechanism for inserting code into DLG class */
    /* MR1        via <<%%lexmember ...>>         */
    /* MR1      This is a consequence of not saving actions         */
    /* MR1                      */
    /* MR1 */    parserClass=0;
    /* MR1 */    lexPrefix=0;
    /* MR1 */    lexAction=0;
    /* MR1 */    lexMember=0;
}


static void act36()
{
    NLA = 34;
    xxputc(zzlextext[0]); zzskip();
}


static void act37()
{
    NLA = 35;
    xxputc('>'); zzskip();
}


static void act38()
{
    NLA = 36;
    xxputc('\\'); zzskip();
}


static void act39()
{
    NLA = 37;
    xxputc(zzlextext[0]); ++zzline; zzskip();
}


static void act40()
{
    NLA = 38;
    zzmode(ACTION_COMMENTS);      /* MR1 */
    xxprintf("%s", &(zzlextext[0])); zzskip();  /* MR1 */
}


static void act41()
{
    NLA = 39;
    zzmode(ACTION_CPP_COMMENTS);      /* MR1 */
    xxprintf("%s", &(zzlextext[0])); zzskip();  /* MR1 */
}


static void act42()
{
    NLA = 40;
    xxputc(zzlextext[0]); zzskip();
}

static unsigned char shift1[257] = {
  0, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 3, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 5, 6, 6, 6, 6, 4, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 1, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 2, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6
};


static void act43()
{
    NLA = 1;
}


static void act44()
{
    NLA = 41;
    zzmode(ACT);          /* MR1 */
    xxprintf("%s", &(zzlextext[0])); zzskip();  /* MR1 */
}


static void act45()
{
    NLA = 42;
    zzline++; xxputc(zzlextext[0]); zzskip();
}


static void act46()
{
    NLA = 43;
    xxputc(zzlextext[0]); zzskip();
}

static unsigned char shift2[257] = {
  0, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 3, 4, 4, 3, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 1, 4, 4, 4, 4, 2, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4
};


static void act47()
{
    NLA = 1;
}


static void act48()
{
    NLA = 44;
    zzmode(ACT); zzline++;      /* MR1 */
    xxprintf("%s", &(zzlextext[0])); zzskip();  /* MR1 */
}


static void act49()
{
    NLA = 45;
    xxputc(zzlextext[0]); zzskip();
}

static unsigned char shift3[257] = {
  0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 1, 2, 2, 1, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2
};

#define DfaStates 94
typedef unsigned char DfaState;

static DfaState st0[42] = {
  1, 2, 3, 4, 5, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 7, 8, 9, 10,
  11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
  6, 94
};

static DfaState st1[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st2[42] = {
  94, 21, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st3[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st4[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st5[42] = {
  94, 94, 94, 94, 22, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st6[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st7[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 23, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st8[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 24, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st9[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st10[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st11[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st12[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st13[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st14[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st15[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st16[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st17[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st18[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st19[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st20[42] = {
  94, 25, 26, 25, 25, 25, 25, 25, 25, 27,
  28, 25, 25, 29, 25, 25, 30, 25, 25, 25,
  25, 25, 25, 31, 32, 32, 25, 25, 25, 25,
  25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
  25, 94
};

static DfaState st21[42] = {
  94, 21, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st22[42] = {
  94, 94, 94, 94, 94, 33, 33, 33, 33, 33,
  33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
  33, 33, 33, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st23[42] = {
  94, 94, 94, 94, 34, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st24[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st25[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st26[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st27[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st28[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st29[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st30[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st31[42] = {
  94, 94, 94, 94, 94, 94, 94, 35, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  35, 94, 94, 36, 36, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st32[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 37, 37, 37, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st33[42] = {
  94, 94, 94, 94, 94, 38, 38, 38, 38, 38,
  38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
  38, 38, 38, 38, 38, 38, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st34[42] = {
  94, 94, 94, 94, 39, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st35[42] = {
  94, 94, 94, 94, 94, 94, 40, 94, 94, 40,
  94, 40, 40, 94, 94, 94, 94, 94, 94, 40,
  94, 40, 94, 40, 40, 40, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st36[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 36, 36, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st37[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 37, 37, 37, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st38[42] = {
  94, 94, 94, 94, 94, 38, 38, 38, 38, 38,
  38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
  38, 38, 38, 38, 38, 38, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st39[42] = {
  94, 94, 94, 94, 94, 41, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 42, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st40[42] = {
  94, 94, 94, 94, 94, 94, 40, 94, 94, 40,
  94, 40, 40, 94, 94, 94, 94, 94, 94, 40,
  94, 40, 94, 40, 40, 40, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st41[42] = {
  94, 94, 94, 94, 94, 94, 43, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st42[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 44, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st43[42] = {
  94, 94, 94, 94, 94, 94, 94, 45, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st44[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  46, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st45[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 47, 94,
  94, 48, 94, 94, 94, 94, 94, 49, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st46[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 50, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st47[42] = {
  94, 94, 94, 94, 94, 94, 51, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st48[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 52, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st49[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  53, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st50[42] = {
  94, 94, 94, 94, 94, 94, 54, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st51[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 55, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st52[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 56, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st53[42] = {
  94, 94, 94, 94, 94, 94, 57, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st54[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  58, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st55[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 59,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st56[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 60, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st57[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 61,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st58[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 62, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st59[42] = {
  94, 94, 94, 94, 94, 94, 63, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st60[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 64, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st61[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 65, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st62[42] = {
  94, 94, 94, 94, 94, 66, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st63[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  67, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st64[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 68, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st65[42] = {
  94, 94, 94, 94, 94, 94, 94, 69, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st66[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 70, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st67[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st68[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st69[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st70[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 71, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st71[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 72, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st72[42] = {
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94, 94, 94, 94, 94, 94, 94, 94, 94,
  94, 94
};

static DfaState st73[8] = {
  74, 75, 76, 77, 78, 79, 79, 94
};

static DfaState st74[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st75[8] = {
  94, 80, 94, 94, 94, 94, 94, 94
};

static DfaState st76[8] = {
  94, 81, 94, 94, 94, 94, 94, 94
};

static DfaState st77[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st78[8] = {
  94, 94, 94, 94, 82, 83, 94, 94
};

static DfaState st79[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st80[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st81[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st82[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st83[8] = {
  94, 94, 94, 94, 94, 94, 94, 94
};

static DfaState st84[6] = {
  85, 86, 87, 88, 87, 94
};

static DfaState st85[6] = {
  94, 94, 94, 94, 94, 94
};

static DfaState st86[6] = {
  94, 94, 89, 94, 94, 94
};

static DfaState st87[6] = {
  94, 94, 94, 94, 94, 94
};

static DfaState st88[6] = {
  94, 94, 94, 94, 94, 94
};

static DfaState st89[6] = {
  94, 94, 94, 94, 94, 94
};

static DfaState st90[4] = {
  91, 92, 93, 94
};

static DfaState st91[4] = {
  94, 94, 94, 94
};

static DfaState st92[4] = {
  94, 94, 94, 94
};

static DfaState st93[4] = {
  94, 94, 94, 94
};


DfaState *dfa[94] = {
  st0,
  st1,
  st2,
  st3,
  st4,
  st5,
  st6,
  st7,
  st8,
  st9,
  st10,
  st11,
  st12,
  st13,
  st14,
  st15,
  st16,
  st17,
  st18,
  st19,
  st20,
  st21,
  st22,
  st23,
  st24,
  st25,
  st26,
  st27,
  st28,
  st29,
  st30,
  st31,
  st32,
  st33,
  st34,
  st35,
  st36,
  st37,
  st38,
  st39,
  st40,
  st41,
  st42,
  st43,
  st44,
  st45,
  st46,
  st47,
  st48,
  st49,
  st50,
  st51,
  st52,
  st53,
  st54,
  st55,
  st56,
  st57,
  st58,
  st59,
  st60,
  st61,
  st62,
  st63,
  st64,
  st65,
  st66,
  st67,
  st68,
  st69,
  st70,
  st71,
  st72,
  st73,
  st74,
  st75,
  st76,
  st77,
  st78,
  st79,
  st80,
  st81,
  st82,
  st83,
  st84,
  st85,
  st86,
  st87,
  st88,
  st89,
  st90,
  st91,
  st92,
  st93
};


DfaState accepts[95] = {
  0, 1, 2, 3, 4, 33, 33, 33, 33, 13,
  14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
  0, 2, 5, 11, 12, 32, 31, 30, 29, 27,
  28, 24, 26, 6, 0, 0, 24, 26, 6, 0,
  25, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 7, 8, 10,
  0, 0, 9, 0, 34, 36, 38, 39, 42, 42,
  35, 37, 41, 40, 0, 43, 46, 46, 45, 44,
  0, 47, 48, 49, 0
};

void (*actions[50])() = {
  zzerraction,
  act1,
  act2,
  act3,
  act4,
  act5,
  act6,
  act7,
  act8,
  act9,
  act10,
  act11,
  act12,
  act13,
  act14,
  act15,
  act16,
  act17,
  act18,
  act19,
  act20,
  act21,
  act22,
  act23,
  act24,
  act25,
  act26,
  act27,
  act28,
  act29,
  act30,
  act31,
  act32,
  act33,
  act34,
  act35,
  act36,
  act37,
  act38,
  act39,
  act40,
  act41,
  act42,
  act43,
  act44,
  act45,
  act46,
  act47,
  act48,
  act49
};

static DfaState dfa_base[] = {
  0,
  73,
  84,
  90
};

static unsigned char *b_class_no[] = {
  shift0,
  shift1,
  shift2,
  shift3
};



#define ZZSHIFT(c) (b_class_no[zzauto][1+c])
#define MAX_MODE 4
#include "dlgauto.h"
