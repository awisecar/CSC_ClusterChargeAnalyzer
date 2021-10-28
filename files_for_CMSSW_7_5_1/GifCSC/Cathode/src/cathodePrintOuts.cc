#include "GifCSC/Cathode/interface/cathodePrintOuts.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigi.h"

#include <stdio.h>

void CLCT_print(const CSCCLCTDigi& clctdigi){
	printf (" >>>>> CLCT: valid:%2d   quality:%2d   pattern:%2d   inStr:%3d   cfeb:%2d   keyStr:%3d   BX:%4d   fullBX:%5d\n",
		clctdigi.isValid(),     // check CLCT validity (1 - valid CLCT)
		clctdigi.getQuality(),  // return quality of a pattern (number of layers hit!)
		clctdigi.getPattern(),  // return pattern
		clctdigi.getStrip(),    // return halfstrip that goes from 0 to 31 
		clctdigi.getCFEB(),     // return Key CFEB ID

		clctdigi.getKeyStrip(), 
		/// Convert strip_ and cfeb_ to keyStrip. Each CFEB has up to 16 strips
		/// (32 halfstrips). There are 5 cfebs.  The "strip_" variable is one
		/// of 32 halfstrips on the keylayer of a single CFEB, so that
		/// Distrip   = (cfeb*32 + strip)/4.
		/// Halfstrip = (cfeb*32 + strip).
		/// Always return halfstrip number since this is what is stored in
		/// the correlated LCT digi.  For distrip patterns, the convention is
		/// the same as for persistent strip numbers: low halfstrip of a distrip.
		/// SV, June 15th, 2006.

		clctdigi.getBX(),       // return BX
		clctdigi.getFullBX());  // return 12-bit full BX.
}
