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
 * \file proto.h
 * function prototypes
 */


#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdio.h>
// for Entry
#include "hash.h"

                           /* V a r i a b l e s */

extern int tp;
extern Junction *SynDiag;
extern char Version[];
extern char VersionDef[];
extern void (*fpPrint[])();
extern struct _set (*fpReach[])();
extern struct _tree *(*fpTraverse[])();
extern void (**fpTrans)();
extern void (**fpJTrans)();
extern int BlkLevel;
extern int CurFile;
extern char *CurRule;
extern RuleEntry *CurRuleNode;
extern ListNode *CurAstLabelsInActions;
extern ListNode *ContextGuardPredicateList;
extern int numericActionLabel;
extern ListNode *NumericPredLabels;
extern char *FileStr[];
extern int NumFiles;
extern int EpToken;
extern int WildCardToken;
extern Entry  **Tname,
        **Texpr,
        **Rname,
        **Fcache,
        **Tcache,
        **Elabel,
        **Sname,
        **Pname;
extern ListNode *ExprOrder;
extern ListNode **Cycles;
extern int TokenNum;
extern int LastTokenCounted;
extern ListNode *BeforeActions, *AfterActions, *LexActions;

/* MR1                                              */
/* MR1  11-Apr-97 Provide mechanism for inserting code into DLG class     */
/* MR1        via #lexmember <<....>> & #lexprefix <<...>>              */
/* MR1                      */

extern ListNode *LexMemberActions;
extern ListNode *LexPrefixActions;

extern int Save_argc;
extern char **Save_argv;
extern ListNode *eclasses, *tclasses;
extern char *HdrAction;
extern char *FirstAction;
extern FILE *ErrFile;
extern char *RemapFileName;
extern char *ErrFileName;
extern char *DlgFileName;
extern char *DefFileName;
extern char *ModeFileName;
extern int NumRules;
extern Junction **RulePtr;
extern int LL_k;
extern int CLL_k;
extern char *decodeJType[];
extern int PrintOut;
extern int PrintAnnotate;
extern int CodeGen;
extern int LexGen;
extern int GenAST;
extern int GenANSI;
extern int **FoStack;
extern int **FoTOS;
extern int GenExprSetsOpt;
extern FILE *DefFile;
extern int CannotContinue;
extern int GenCR;
extern int GenLineInfo;
extern int GenLineInfoMS;
extern int action_file, action_line;
extern int TraceGen;
extern int CurAmbigAlt1, CurAmbigAlt2, CurAmbigline, CurAmbigfile;
extern char *CurAmbigbtype;
extern int elevel;
extern int GenEClasseForRules;
extern FILE *input, *output;
extern char **TokenStr, **ExprStr;
extern int CurrentLexClass, NumLexClasses;
extern LClass lclass[];
extern char *CurRetDef;
extern char *CurParmDef;
extern int OutputLL_k;
extern int TreeResourceLimit;
extern int DemandLookahead;
extern char *RulePrefix;
extern int GenStdPccts;
extern char *stdpccts;
extern int ParseWithPredicates;
extern int PURIFY;
extern set MR_CompromisedRules;
extern int MR_usingPredNames;
extern int MR_ErrorSetComputationActive;
extern char *MR_AmbAidRule;
extern int   MR_AmbAidLine;
extern int   MR_AmbAidMultiple;
extern int MR_AmbAidDepth;
extern int MR_skipped_e3_report;
extern int MR_Inhibit_Tokens_h_Gen;
extern int NewAST;
extern int tmakeInParser;
extern int AlphaBetaTrace;
extern int MR_BlkErr;
extern int MR_AlphaBetaWarning;
extern int MR_AlphaBetaMessageCount;
extern int MR_MaintainBackTrace;
extern int MR_BadExprSets;
extern int FoundGuessBlk;
extern int FoundException;
extern int FoundAtOperator;
extern int FoundExceptionGroup;
extern int WarningLevel;
extern int UseStdout;
extern int TabWidth;
extern int pLevel;
extern int pAlt1;
extern int pAlt2;
extern int AImode;
extern int HoistPredicateContext;
extern int MRhoisting;
extern int MRhoistingk;
extern int MR_debugGenRule;
extern int GenCC;
extern char *ParserName;
extern char *StandardSymbols[];
extern char *ASTSymbols[];
extern set reserved_positions;
extern set all_tokens;
extern set imag_tokens;
extern set tokclasses;
extern ListNode *ForcedTokens;
extern int *TokenInd;
extern FILE *Parser_h, *Parser_c;
extern char CurrentClassName[];
extern int no_classes_found;
extern char Parser_h_Name[];
extern char Parser_c_Name[];
extern char MRinfoFile_Name[];
extern FILE *MRinfoFile;
extern int MRinfo;
extern int MRinfoSeq;
extern int InfoP;
extern int InfoT;
extern int InfoF;
extern int InfoM;
extern int InfoO;
extern int PotentialSuppression;
extern int PotentialDummy;
extern int TnodesInUse;
extern int TnodesPeak;
extern int TnodesReportThreshold;
extern int TnodesAllocated;
extern char *ClassDeclStuff;
extern char *BaseClassName;
extern ListNode *class_before_actions, *class_after_actions;
extern char *UserTokenDefsFile;
extern int UserDefdTokens;
extern ListNode *MetaTokenNodes;
extern char *OutputDirectory;
extern int DontCopyTokens;
extern int LTinTokenAction;
extern set AST_nodes_refd_in_actions;
extern int CurBlockID;
extern int CurAltNum;
extern Junction *CurAltStart;
extern Junction *OuterAltStart;
extern ExceptionGroup *DefaultExGroup;
extern int ContextGuardTRAV;
extern Junction *MR_RuleBlkWithHalt;
extern PointerStack MR_BackTraceStack;
extern PointerStack MR_PredRuleRefStack;
extern PointerStack MR_RuleBlkWithHaltStack;

extern int tokenActionActive;

extern FILE * NextFile( void );
extern char * outname( char * );
extern char *OutMetaName(char *);

/* MR20 G. Hobbelt  Create proper externs for dlg variables */

extern set attribsRefdFromAction;
extern int inAlt;
extern int UsedOldStyleAttrib;
extern int UsedNewStyleLabel;

#define MAX_BLK_LEVEL 100
extern int     CurBlockID_array[MAX_BLK_LEVEL];
extern int     CurAltNum_array[MAX_BLK_LEVEL];

#endif
