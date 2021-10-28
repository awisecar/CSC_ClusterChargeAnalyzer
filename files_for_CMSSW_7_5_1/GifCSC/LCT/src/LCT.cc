#include "GifCSC/LCT/interface/LCT.h"
#include "GifCSC/LCT/interface/lctPrintOuts.h"
#include <iostream>

LCT::LCT(short unsigned                                  chamber,
		edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> lcttoken,
		TDirectory *                                     outputroot,
		short unsigned                                   debuglevel)
                      : chamberType(chamber), commonCurrentEvtN(-1), evtsProccededLCTs(0),
						debug(debuglevel){
	// token
	lct_token = lcttoken;

	// histos
	lct_histos = NULL; 
	histogramming = false;
	if(outputroot!=NULL){
		if(debug) std::cout << "LCT::LCT: creating histogram subdirectory in directory " << outputroot->GetPath() << std::endl;
		TDirectory *outdir = outputroot->mkdir("hLCT");
		if(outdir->cd()){
			lct_histos    = new basicLCTHistos(outdir, debug);
			histogramming = true;
		}else{std::cout << "LCT can not create directory for basic LCT historgrams in file " << outputroot->GetPath() << std::endl;}
	}else{
		std::cout << "LCT will not save basic historgrams since no directory was prepared!\n";
	};
}


bool LCT::end(){
	bool ok = true;
	if(histogramming){
		ok = (ok && lct_histos->write());
	};
	if(debug) std::cout << "LCT::end(): writing histogram status: " << ok << std::endl;
	return ok;
}



LCT::~LCT(){
	LCT::end();
	delete lct_histos; // histos should be already written
}



//bool LCT::lctInEvent(const edm::Event& iEvent){
const std::vector<CSCCorrelatedLCTDigi>& LCT::lctInEvent(const edm::Event& iEvent){
//https://cmssdt.cern.ch/lxr/source/DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigi.h?%21v=CMSSW_7_5_8
	lct_v.clear();
	edm::Handle<CSCCorrelatedLCTDigiCollection> lct;
	iEvent.getByToken(lct_token, lct);

	short unsigned int   N = 0;
	short unsigned int dBX = 0;
	for (CSCCorrelatedLCTDigiCollection::DigiRangeIterator lctit=lct->begin(); lctit!=lct->end(); ++lctit) {
		CSCDetId id = (CSCDetId)(*lctit).first;
		if(id.chamber()!=chamberType) continue;
		if(debug) std::cout   << " LCT from : chamber " << id.chamber()
							  << " station " << id.station()
							  << " ring    " << id.ring()
							  << " layer   " << id.layer() << std::endl;
		std::vector<CSCCorrelatedLCTDigi>::const_iterator digiItr = (*lctit).second.first;
		std::vector<CSCCorrelatedLCTDigi>::const_iterator last    = (*lctit).second.second;
		if(digiItr!=last) evtsProccededLCTs++;
		short int       lastBX = -100;
		while (digiItr != last) {
			lct_v.push_back(*digiItr);
			if(debug)          LCT_print(*digiItr);
			if(histogramming)  lct_histos->fillLCTHistos(*digiItr);

			short int bx = (*digiItr).getBX();
			if(N>0 and bx!=lastBX)
				if(abs(bx-lastBX)<dBX) dBX=lastBX;

			lastBX = bx;
			++digiItr; N++;
		};
	}; // end loop CSCLCTDigiCollection
	if(histogramming) lct_histos->fillLCTHistos(N, dBX);

	return lct_v;
};

