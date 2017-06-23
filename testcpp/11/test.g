/* This is test.g which tests multiple scanners/parsers; DLG-based scanner;
 * also, we test multiple lexical classes.
 */
<<
#include "Lexer.h"
typedef ANTLRCommonToken ANTLRToken;
#include "PBlackBox.h"

int main()
{
	ParserBlackBox<Lexer, Include, ANTLRToken> p(stdin);
	p.parser()->input();
	return 0;
}
>>

#token "[\ \t\n]+"	<<skip();>>

#lexclass START

class Include {

<<
/* this is automatically defined to be a member function of Include::
 * since it is within the "class {...}" boundaries.
 */
private:
char *stripquotes(ANTLRChar *s)
{
	s[strlen(s)-1] = '\0';
	return &s[1];
}
>>

input
	:	( cmd | include )* "@"
	;

cmd	:	"print"
		(	NUMBER		<<printf("%s\n", $1->getText());>>
		|	STRING		<<printf("%s\n", $1->getText());>>
		)
	;

include
	:	"#data" STRING
		<<{
		FILE *f;
		f = fopen(stripquotes($2->getText()), "r");
		if ( f==NULL ) {fprintf(stderr, "can't open %s\n", $2->getText()+1);}
		else {
			ANTLRTokenPtr aToken = new ANTLRToken;
			DLGFileInput in(f);
			Lexer scan(&in);
			scan.setToken(mytoken(aToken));
			scan.mode(Lexer::DATA);
			ANTLRTokenBuffer pipe(&scan);
			Include parser(&pipe);
			parser.init();
			parser.data();
		}
		}>>
	;

#lexclass DATA

#token "[\ \t\n]+"	<<skip();>>

data:	"0x[0-9]+" ":" "0x[0-9]+"
		<<printf("data %s\n", $1->getText());>>
	;

}

#lexclass START

#token STRING	"\" [a-zA-Z0-9_.,\ \t]+ \""
#token NUMBER	"[0-9]+"
