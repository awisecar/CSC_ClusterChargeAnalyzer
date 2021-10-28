#ifndef csc_status4gif_h
#define csc_status4gif_h

#include "FWCore/Framework/interface/EDConsumerBase.h"       // edm::EDGetTokenT
#include "FWCore/ParameterSet/interface/ParameterSet.h"      // edm::ParameterSet
#include "FWCore/Framework/interface/Event.h"                // edm::Event
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
//#include "TH1F.h"
//#include "TH2F.h"

class StatusDigis {
 public:
	StatusDigis(short unsigned                           chamber,
		edm::EDGetTokenT<CSCTMBStatusDigiCollection>     tmbstattoken,
		// more tokens to be added
		TDirectory *                                     outputroot,
		short unsigned                                   debuglevel);
	~StatusDigis();
private:
	int  chamberType;  // 1 - ME1/1; 2 - ME2/1; >10 - P5 (not yet); signed because of CSCDetId.chamber is signed
	long commonCurrentEvtN; // !!!TBD- not yet; current event N, -1 if not initialized
	unsigned int  evtsProccededStatusDigis;   // event counter StatusDigis
	short unsigned int debug;            // 0 - no debug
	edm::EDGetTokenT<CSCTMBStatusDigiCollection> tmbstat_token;
	// more status digi collections to be added
private:
	//basicLCTHistos * lct_histos;
	bool histogramming;
	std::vector<CSCTMBStatusDigi> tmbstat_v;
public:
	const std::vector<CSCTMBStatusDigi>& tmbStatInEvent(const edm::Event& iEvent);
	bool end(); // basicCLCTHistos->write to be called here

};
#endif
