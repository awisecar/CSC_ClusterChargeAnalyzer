#include <utility>
#include <iostream>

#include "TDirectory.h"
#include "TH1F.h"

#include "GifCSC/StatusDigis/interface/basicStatusHistos.h"
#include "GifCSC/basics/interface/basicHistosNaming.h"



basicStatusHistos::basicStatusHistos(TDirectory * dout, const int debug):basicHistos(dout, debug){
	if(debuglevel>1){
		std::cout << " basicStatusHistos::basicStatusHistos   : directory pointer " << &outdir << "; directory name: ";
		std::cout << outdir->GetPath() << std::endl;
	};
	book();
	if(debuglevel)std::cout << " basicStatusHistos::basicStatusHistos	   : => completed " << std::endl;
}



bool basicStatusHistos::book(){
	outdir->cd(); // important
	TH1F * h;
	TString htitle, hdescr, hfunc;

	// 0
	htitle="alctINclct"; hfunc="TMBstatus"; hdescr="ALCT in CLCT window";
	h = new TH1F(htitle.Data(), htitle.Data(), 17, -1, 16);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	// 1
	htitle="l1INtmp"; hfunc="TMBstatus"; hdescr="L1 in TMB window";
	h = new TH1F(htitle.Data(), htitle.Data(), 17, -1, 16);
	histosWithDescritions.push_back(std::make_pair(h, new basicHistosNaming(htitle.Data(), htitle.Data(), hfunc.Data(), hdescr.Data())));

	if(debuglevel){
		std::cout << " basicStatusHistos::book         : booking ends with following content in the directory :" << std::endl;
		outdir->Print(); // does not work in CMSSW??!
		for (std::vector<std::pair<TH1F*, basicHistosNaming*>>::iterator it=histosWithDescritions.begin(); it!=histosWithDescritions.end(); ++it)
			(*it).second->Print();
	};
	return true;
};

bool basicStatusHistos::fillTMBStatusHistos(const CSCTMBStatusDigi& tmbdigi){
	return true;
}
