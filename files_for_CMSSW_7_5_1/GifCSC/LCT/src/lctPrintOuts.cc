#include "GifCSC/LCT/interface/lctPrintOuts.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"

#include <stdio.h>

void LCT_print(const CSCCorrelatedLCTDigi& lctdigi){
	printf ("\t\t LCT: v:%2d   q:%2d   p:%2d    kWG:%4d             kStr:%3d   bx:%4d\n",
			lctdigi.isValid(), // v
			lctdigi.getQuality(),  // q
			lctdigi.getPattern(),  // p
			lctdigi.getKeyWG(),    // kWG
			lctdigi.getStrip(),    // kStr          - halfstrip
			lctdigi.getBX());
}
