#ifndef csc_lct4gif_h
#define csc_lct4gif_h

#include "FWCore/Framework/interface/EDConsumerBase.h"       // edm::EDGetTokenT
#include "FWCore/ParameterSet/interface/ParameterSet.h"      // edm::ParameterSet
#include "FWCore/Framework/interface/Event.h"                // edm::Event
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h"
#include "TH1F.h"
#include "TH2F.h"

#include "GifCSC/LCT/interface/basicLCTHistos.h"
class LCT {
 public:
	LCT(short unsigned                                   chamber,
		edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> lcttoken,
		TDirectory *                                     outputroot,
		short unsigned                                   debuglevel);
	~LCT();
private:
	int  chamberType;  // 1 - ME1/1; 2 - ME2/1; >10 - P5 (not yet); signed because of CSCDetId.chamber is signed
	long commonCurrentEvtN; // !!!TBD- not yet; current event N, -1 if not initialized
	unsigned int  evtsProccededLCTs;    // event counter CLCT
	short unsigned int debug;            // 0 - no debug
	edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> lct_token;
private:
	basicLCTHistos * lct_histos;
	bool histogramming;
	std::vector<CSCCorrelatedLCTDigi> lct_v;
public:
	const std::vector<CSCCorrelatedLCTDigi>& lctInEvent(const edm::Event& iEvent);
	bool end(); // basicCLCTHistos->write to be called here

};
#endif
