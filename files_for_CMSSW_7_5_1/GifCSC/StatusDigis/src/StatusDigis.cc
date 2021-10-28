#include "EventFilter/CSCRawToDigi/interface/CSCTMBHeader2013.h" // ME1/1
#include "EventFilter/CSCRawToDigi/interface/CSCTMBHeader2007.h" // ME2/1
#include "EventFilter/CSCRawToDigi/interface/CSCTMBHeader.h"

#include "GifCSC/StatusDigis/interface/StatusDigis.h"
#include "GifCSC/StatusDigis/interface/statusPrintOuts.h"
#include <iostream>

StatusDigis::StatusDigis(short unsigned                  chamber,
		edm::EDGetTokenT<CSCTMBStatusDigiCollection>     tmbstattoken,
		// more tokens to be added
		TDirectory *                                     outputroot,
		short unsigned                                   debuglevel)
				: chamberType(chamber), commonCurrentEvtN(-1), evtsProccededStatusDigis(0),  debug(debuglevel){
	// tokens
	tmbstat_token = tmbstattoken;

	/*
	// histos
	lct_histos = NULL; histogramming = false;
	if(outputroot!=NULL){
		if(debug) std::cout << "creating histogram subdirectory in directory " << outputroot->GetPath() << std::endl;
		TDirectory *outdir = outputroot->mkdir("hLCT");
		if(outdir->cd()){
			lct_histos    = new basicLCTHistos(outdir, debug);
			histogramming = true;
		}else{std::cout << "LCT can not create directory for basic LCT historgrams in file " << outputroot->GetPath() << std::endl;}
	}else{
		std::cout << "LCT will not save basic historgrams since no directory was prepared!\n";
	};*/
}


bool StatusDigis::end(){
	bool ok = true;
	/*
	if(histogramming){
		ok = (ok && lct_histos->write());
	};
	*/
	return ok;
}



StatusDigis::~StatusDigis(){
	//delete lct_histos; // histos should be already written
}



const std::vector<CSCTMBStatusDigi>& StatusDigis::tmbStatInEvent(const edm::Event& iEvent){
	std::cout << "we are here1\n";
	tmbstat_v.clear();
	edm::Handle<CSCTMBStatusDigiCollection> tmb;
	iEvent.getByToken(tmbstat_token, tmb);
	std::cout << "we are here2\n";

	short unsigned int   N = 0;
	//short unsigned int dBX = 0;

	for (CSCTMBStatusDigiCollection::DigiRangeIterator tmbit=tmb->begin(); tmbit!=tmb->end(); ++tmbit) {
		CSCDetId id = (CSCDetId)(*tmbit).first;
		if(id.chamber()!=chamberType) continue;
		if(debug) std::cout   << " LCT from : chamber " << id.chamber()
							  << " station " << id.station()
							  << " ring    " << id.ring()
							  << " layer   " << id.layer() << std::endl;


		std::vector<CSCTMBStatusDigi>::const_iterator digiItr = (*tmbit).second.first;
		std::vector<CSCTMBStatusDigi>::const_iterator last    = (*tmbit).second.second;
		//if(digiItr!=last) N++;
		//short int       lastBX = -100;
		while (digiItr != last) {
			//lct_v.push_back(*digiItr);

			CSCTMBHeader tmbhead = CSCTMBHeader((*digiItr).header());
			//CSCTMBHeader2013 tmbhead13 = CSCTMBHeader2013((*digiItr).header());
			//CSCTMBHeader2013 tmbhead13 = tmbhead.tmbHeader2013()
//			The value for ALCT location in CLCT matching window is matchWin in this line:
//			https://github.com/cms-sw/cmssw/blob/master/EventFilter/CSCRawToDigi/interface/CSCTMBHeader2013.h#L92
//
//			the L1a location in TMB window is pop_l1a_match_win in this line:
//			https://github.com/cms-sw/cmssw/blob/master/EventFilter/CSCRawToDigi/interface/CSCTMBHeader2013.h#L90

			if(debug){

				std::cout 	<< "\t" << N
						<< "\t" << tmbhead.FirmwareVersion()
						<< "\t" << tmbhead.L1ANumber()
						//<< "\t" << (*digiItr).header()
							<< "\ta " << tmbhead.ALCTMatchTime()
							<< "\tt " << tmbhead.TMBMatch();
				if(id.chamber()==1){
					std::cout << "\ta13 " << tmbhead.tmbHeader2013().bits.matchWin
							  << "\tt13 " << tmbhead.tmbHeader2013().bits.pop_l1a_match_win;
				}else if (id.chamber()==2){
					std::cout << "\ta07 " << tmbhead.tmbHeader2007().bits.matchWin
							  << "\tt07 " << tmbhead.tmbHeader2007().bits.pop_l1a_match_win;
				};
				std::cout << std::endl;

			}
				//LCT_print(*digiItr);
			//if(histogramming)  lct_histos->fillLCTHistos(*digiItr);
			/*
			short int bx = (*digiItr).getBX();
			if(N>0 and bx!=lastBX)
				if(abs(bx-lastBX)<dBX) dBX=lastBX;

			lastBX = bx;
			*/
			++digiItr; N++;
		};
	}; // end loop CSCLCTDigiCollection
	//if(histogramming) lct_histos->fillLCTHistos(N, dBX);

	return tmbstat_v;
};

