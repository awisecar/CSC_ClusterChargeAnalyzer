#ifndef statusHistos_h
#define statusHistos_h

#include "GifCSC/basics/interface/basicHistos.h"
#include "DataFormats/CSCDigi/interface/CSCTMBStatusDigi.h"

class basicStatusHistos:public basicHistos{
public:
	basicStatusHistos(TDirectory * dout, const int debug=0);
	bool fillTMBStatusHistos(const CSCTMBStatusDigi& tmbdigi);
private:
	bool book(); // the only what has to be rewritten from basicHistos
};
#endif
