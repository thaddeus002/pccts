/* MyTokenBuffer.h */

#include "ATokenBuffer.h"

class MyLexer : public ANTLRTokenStream {
private:
  int c;
public:
  MyLexer();
  virtual ANTLRAbstractToken *getToken();
};
