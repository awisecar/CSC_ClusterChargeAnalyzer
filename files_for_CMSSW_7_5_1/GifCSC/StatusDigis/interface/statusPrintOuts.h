#ifndef statusPrintOuts_h
#define statusPrintOuts_h

#include "DataFormats/CSCDigi/interface/CSCDMBStatusDigi.h"
#include "DataFormats/CSCDigi/interface/CSCDMBStatusDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCTMBStatusDigi.h"
#include "DataFormats/CSCDigi/interface/CSCTMBStatusDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCDDUStatusDigi.h"
#include "DataFormats/CSCDigi/interface/CSCDDUStatusDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCDCCStatusDigi.h"
#include "DataFormats/CSCDigi/interface/CSCDCCStatusDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTStatusDigi.h"
#include "DataFormats/CSCDigi/interface/CSCALCTStatusDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTStatusDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCDCCFormatStatusDigi.h"
#include "DataFormats/CSCDigi/interface/CSCDCCFormatStatusDigiCollection.h"

// more printouts to be added
void tmbSTATUS_print(const CSCTMBStatusDigi& tmbstatdigi);
#endif
