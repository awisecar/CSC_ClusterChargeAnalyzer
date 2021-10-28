#ifndef csc_cathodeHits_h
#define csc_cathodeHits_h

#include "FWCore/Framework/interface/EDConsumerBase.h"       // edm::EDGetTokenT
#include "FWCore/ParameterSet/interface/ParameterSet.h"      // edm::ParameterSet
#include "FWCore/Framework/interface/Event.h"                // edm::Event
#include "DataFormats/CSCDigi/interface/CSCCLCTDigi.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigi.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"

#include "TDirectory.h"
#include "TH1F.h"
#include "TH2F.h"

#include "GifCSC/Cathode/interface/cathodeHistos.h"

struct CLCTstruct{
	int layer;
	int keyStrip;
	int TB;
};
struct COMPstruct{
	int layer;
	int keyStrip;
	int TB;
};

class cathodeHitsTrigs {

 public:
	cathodeHitsTrigs(short unsigned                               chamber,
					edm::EDGetTokenT<CSCCLCTDigiCollection>       clcttoken,
					edm::EDGetTokenT<CSCComparatorDigiCollection> comptoken,
					edm::EDGetTokenT<CSCStripDigiCollection>      striptoken,
					TDirectory *                                  outputroot,
					short unsigned                                debuglevel);
	~cathodeHitsTrigs();

private:
	int  chamberType;  // 1 - ME1/1; 2 - ME2/1; >10 - P5 (not yet); signed because of CSCDetId.chamber is signed
	long commonCurrentEvtN; // !!!TBD - not used yet; current event N, -1 if not initialized
	unsigned int  evtsProccededCLCTs;    // event counter CLCT
	double nStripCompDigiMatch;
	double nStripCompDigiMatchReducedVolume;
	short unsigned int debug;            // 0 - no debug
	edm::EDGetTokenT<CSCCLCTDigiCollection>       clct_token;
	edm::EDGetTokenT<CSCComparatorDigiCollection> comp_token;
	edm::EDGetTokenT<CSCStripDigiCollection>     strip_token;
private:
	std::vector<CLCTstruct>  clcts;
	std::vector<COMPstruct>  comps;
	cathodeHistos * cathode_histos;
	bool histogramming;

public:
	bool clctInEvent(const edm::Event& iEvent);
	bool compInEvent(const edm::Event& iEvent);
	bool stripInEvent(const edm::Event& iEvent, bool givenCOMPs = true);
	bool end(); // basicCLCTHistos->write to be called here

};
#endif
