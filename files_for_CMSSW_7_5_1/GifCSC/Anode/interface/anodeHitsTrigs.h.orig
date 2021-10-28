#ifndef csc_anodeHits_h
#define csc_anodeHits_h

//#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDConsumerBase.h"       // edm::EDGetTokenT
#include "FWCore/ParameterSet/interface/ParameterSet.h"   // edm::ParameterSet
#include "FWCore/Framework/interface/Event.h"                // edm::Event
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "TH1F.h"
#include "TH2F.h"

class anodeHitsTrigs: public edm::EDConsumerBase{
 public:
	anodeHitsTrigs(short unsigned chamber, edm::EDGetTokenT<CSCALCTDigiCollection> alcttoken, short unsigned debuglevel, bool test11);
	~anodeHitsTrigs();
private:
	int  chamberType;                // 1 - ME1/1; 2 - ME2/1; >10 - P5 (not yet); signed because of CSCDetId.chamber is signed
	long 	commonCurrentEvtN;       // current event N, -1 if not initialized
	unsigned int evtsProceededWGs;   // event counter anode hits
	unsigned int evtsProccededALCTs; // event counter ALCT
	short unsigned int debug;            // 0 - no debug
	edm::EDGetTokenT<CSCWireDigiCollection> wires_token;
	edm::EDGetTokenT<CSCALCTDigiCollection> alct_token;
private:
	bool test11norm;
	const short unsigned int nOfWGs1 = 48;
	const short unsigned int nOfWGs2 = 112;
	float * wL;        // WG length array [cm], ME11gif given explicitly
	float * wN;        // Number of wires per a WG, ME11gif given explicitly
	float wNorm;       // normalization factor for wire occupancies (test11 vs test27s ie number of CFEBs)
	bool fillWL;       // flag to fill wirelength array, true by constructor, false after filling
	// debug vars
	float * wH1;       // debug vars: array with wirehits (L1)

private:
	TH1F  * hNofAhit[3];     // number of anode hits from CSCWireDigi per event [0] - all hits, [1] - "single" hits, [3] - number of fired layers
	TH1F  * hNofAhitL[6][3]; // number of anode hits from CSCWireDigi per layer per event [0] - all hits, [1] - "single" hits, [3] - fired layer
	TH1F  * hAhitL[6][2];    // anode hits from WGs: [0] - all hits, [1] - "single" hits
	TH1F  * hAhitLNorm[6][2];// anode hits from WGs normalized to length of WGs (wL): [0] - all hits, [1] - "single" hits
	TH2F  * h2dNofAhitWG[6];   // number of "simultaneous" hits from a WG (first WG is assigned to be actual one)
	TH2F  * hALength1;       // anode wire length

private:
	bool bookAnodeHistos();
	bool writeAnodeHistos();
	bool cleanAnodeHistos();
public:
	bool alctInEvent(const edm::Event& iEvent);
};
#endif
