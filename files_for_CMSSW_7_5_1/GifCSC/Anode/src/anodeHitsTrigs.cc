#include <iostream>
#include "GifCSC/Anode/interface/anodeHitsTrigs.h"

anodeHitsTrigs::anodeHitsTrigs(short unsigned chamber, edm::EDGetTokenT<CSCALCTDigiCollection> alcttoken, short unsigned debuglevel, bool test11)
					 : chamberType(chamber), commonCurrentEvtN(-1), evtsProceededWGs(0), evtsProccededALCTs(0),
					   debug(debuglevel),   test11norm(test11){
	//std::cout << "here1: " << alct_token.isUninitialized() << "\t" << alct_token.index() << std::endl;
	alct_token = alcttoken;
	//std::cout << "here2: " << alct_token.isUninitialized() << "\t" << alct_token.index() << std::endl;
}


anodeHitsTrigs::~anodeHitsTrigs(){

}

bool anodeHitsTrigs::bookAnodeHistos(){
	bool ok = false;
	return ok;
}
bool anodeHitsTrigs::writeAnodeHistos(){
	bool ok = false;
	return ok;
}
bool anodeHitsTrigs::cleanAnodeHistos(){
	bool ok = false;
	return ok;
}
bool anodeHitsTrigs::alctInEvent(const edm::Event& iEvent){
	edm::Handle<CSCALCTDigiCollection> alct;
	iEvent.getByToken(alct_token, alct);
	//if(debug)std::cout << "ALCT handle (anode)" << alct << std::endl;
	for (CSCALCTDigiCollection::DigiRangeIterator j=alct->begin(); j!=alct->end(); ++j) {
		CSCDetId id = (CSCDetId)(*j).first;
		//std::cout << id.chamber() << std::endl;
		if(id.chamber()!=chamberType) continue;
		if(debug) std::cout   <<  "ALCT from : chamber " << id.chamber()
							  << " station " << id.station()
							  << " ring    " << id.ring()
							  << " layer   " << id.layer() << std::endl;
		std::vector<CSCALCTDigi>::const_iterator digiItr = (*j).second.first;
		std::vector<CSCALCTDigi>::const_iterator last    = (*j).second.second;

		if(debug){
			std::cout << "KeyWG - Valid - bx" << std::endl;
			while ( digiItr != last ){
				if(digiItr->isValid())
						std::cout << digiItr->getKeyWG()  << "\t"
								  << digiItr->isValid()   << "\t"
								  << digiItr->getBX()     << std::endl;
				 ++digiItr;
			};
		};
	};
	return true;
}

