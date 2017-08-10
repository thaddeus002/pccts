#ifndef tokens_h
#define tokens_h

/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: antlr.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */

#define zzEOF_TOKEN 1
#define Eof 1
#define QuotedTerm 2
#define Action 34
#define Pred 35
#define PassAction 36
#define WildCard 87
#define LABEL 89
#define Pragma 92
#define FirstSetSymbol 93
#define NonTerminal 100
#define TokenTerm 101
#define ID 148
#define INT 150


void grammar();
void class_def();
void rule(void);
void laction();
void lmember();
void lprefix();
void aPred();
extern Predicate * predOrExpr();
extern Predicate * predAndExpr();
extern Predicate * predPrimary();
void aLexclass();
void error();
void tclass();
void token();
void block(set * toksrefd,set * rulesrefd);
void alt(set * toksrefd,set * rulesrefd);
extern LabelEntry * element_label();
extern Node * element(int old_not,int first_on_line,int use_def_MT_handler);
void default_exception_handler();
extern ExceptionGroup * exception_group();
extern ExceptionHandler * exception_handler();
void enum_file(char * fname);
void defines(char * fname);
void enum_def(char * fname);

#endif

extern SetWordType zzerr1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType setwd1[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType setwd2[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType zzerr12[];
extern SetWordType zzerr13[];
extern SetWordType setwd3[];
extern SetWordType zzerr14[];
extern SetWordType zzerr15[];
extern SetWordType zzerr16[];
extern SetWordType zzerr17[];
extern SetWordType zzerr18[];
extern SetWordType zzerr19[];
extern SetWordType zzerr20[];
extern SetWordType zzerr21[];
extern SetWordType setwd4[];
extern SetWordType zzerr22[];
extern SetWordType zzerr23[];
extern SetWordType zzerr24[];
extern SetWordType zzerr25[];
extern SetWordType zzerr26[];
extern SetWordType setwd5[];
extern SetWordType zzerr27[];
extern SetWordType zzerr28[];
extern SetWordType zzerr29[];
extern SetWordType zzerr30[];
extern SetWordType zzerr31[];
extern SetWordType zzerr32[];
extern SetWordType zzerr33[];
extern SetWordType setwd6[];
extern SetWordType zzerr34[];
extern SetWordType zzerr35[];
extern SetWordType zzerr36[];
extern SetWordType zzerr37[];
extern SetWordType zzerr38[];
extern SetWordType zzerr39[];
extern SetWordType zzerr40[];
extern SetWordType zzerr41[];
extern SetWordType zzerr42[];
extern SetWordType setwd7[];
extern SetWordType zzerr43[];
extern SetWordType zzerr44[];
extern SetWordType zzerr45[];
extern SetWordType zzerr46[];
extern SetWordType zzerr47[];
extern SetWordType zzerr48[];
extern SetWordType zzerr49[];
extern SetWordType zzerr50[];
extern SetWordType zzerr51[];
extern SetWordType zzerr52[];
extern SetWordType zzerr53[];
extern SetWordType setwd8[];
extern SetWordType zzerr54[];
extern SetWordType zzerr55[];
extern SetWordType zzerr56[];
extern SetWordType zzerr57[];
extern SetWordType setwd9[];
extern SetWordType zzerr58[];
extern SetWordType zzerr59[];
extern SetWordType zzerr60[];
extern SetWordType zzerr61[];
extern SetWordType zzerr62[];
extern SetWordType zzerr63[];
extern SetWordType zzerr64[];
extern SetWordType zzerr65[];
extern SetWordType setwd10[];
extern SetWordType setwd11[];
