#include <utility>
#include <iostream>

#include "TDirectory.h"
#include "TH1F.h"

#include "GifCSC/LCT/interface/basicLCTHistos.h"

#include "GifCSC/basics/interface/basicHistosNaming.h"

//!!!TBD!!! does it work for two chambers???

basicLCTHistos::basicLCTHistos(TDirectory * dout, const int debug):basicHistos(dout, debug){
	if(debuglevel>1){
		std::cout << " basicLCTHistos::basicLCTHistos   : directory pointer " << &outdir << "; directory name: ";
		std::cout << outdir->GetPath() << std::endl;
	};
	book();
	if(debuglevel)std::cout << " basicLCTHistos::basicCLCTHistos	   : => completed " << std::endl;
}



bool basicLCTHistos::book(){
	outdir->cd(); // important
	TH1F * h;
	TString htitle, hdescr, hfunc;

	// 0
	htitle="NperEvt"; hfunc="evaluated"; hdescr="number of LCTs per event";
	h = new TH1F(htitle.Data(), htitle.Data(), 11, -1, 10);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 1
	htitle="dBX"; hfunc="evaluated"; hdescr="minDifference between two LCTs from different BX";
	h = new TH1F(htitle.Data(), htitle.Data(), 11, -1, 10);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 2
	htitle="validity"; hfunc="CSCCorrelatedLCTDigi.isValid()"; hdescr="validity";
	h = new TH1F(htitle.Data(), htitle.Data(), 3, -1, 2);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 3
	htitle="halfstripN";  hfunc="CSCCorrelatedLCTDigi.getStrip()"; hdescr="absolute halfstrip number";
	h = new TH1F(htitle.Data(), htitle.Data(), 251, -1, 250);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 4
	htitle="quality";  hfunc="CSCCorrelatedLCTDigi.getQuality()"; hdescr="quality";
	h = new TH1F(htitle.Data(),htitle.Data(), 101, -1, 100);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 5
	htitle="pattern";  hfunc="CSCCorrelatedLCTDigi.getPattern()"; hdescr="pattern";
	h = new TH1F(htitle.Data(),htitle.Data(), 21, -1, 20);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 6
	htitle="BX";  hfunc="CSCCorrelatedLCTDigi.getBX()"; hdescr="relative BX number";
	h = new TH1F(htitle.Data(),htitle.Data(), 20, -10, 10);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 8
	htitle="WG";  hfunc="CSCCorrelatedLCTDigi.getKeyWG()"; hdescr="ALCT key WG";
	h = new TH1F(htitle.Data(),htitle.Data(), 210, -10, 200);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	if(debuglevel){
		std::cout << " basicLCTHistos::book         : booking ends with following content in the directory :" << std::endl;
		outdir->Print(); // does not work in CMSSW??!
		for (std::vector<std::pair<TH1F*, basicHistosNaming*>>::iterator it=histosWithDescritions.begin(); it!=histosWithDescritions.end(); ++it)
			(*it).second->Print();
	};
	return true;
};



bool basicLCTHistos::fillLCTHistos(const CSCCorrelatedLCTDigi& lctdigi){

	if(histosWithDescritions.size()<8) return false;

	histosWithDescritions.at(2).first->Fill(lctdigi.isValid());
	histosWithDescritions.at(3).first->Fill(lctdigi.getStrip());
	histosWithDescritions.at(4).first->Fill(lctdigi.getQuality());
	histosWithDescritions.at(5).first->Fill(lctdigi.getPattern());
	histosWithDescritions.at(6).first->Fill(lctdigi.getBX());
	histosWithDescritions.at(7).first->Fill(lctdigi.getKeyWG());
	return true;
};

bool basicLCTHistos::fillLCTHistos(short int NclctEvt=-1, short int dBX=-1){
	if(histosWithDescritions.size()<2) return false;

	histosWithDescritions.at(0).first->Fill(NclctEvt);
	histosWithDescritions.at(1).first->Fill(dBX);
	return true;
}
