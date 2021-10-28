#ifndef lctHistos_h
#define lctHistos_h

#include "GifCSC/basics/interface/basicHistos.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"

class basicLCTHistos:public basicHistos{
public:
	basicLCTHistos(TDirectory * dout, const int debug=0);
	bool fillLCTHistos(const CSCCorrelatedLCTDigi& lctdigi);
	bool fillLCTHistos(short int NlctEvt, short int dBX);
private:
	bool book(); // the only what has to be rewritten from basicHistos
};
#endif
